/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-12-26
 * Description : a kipi plugin to import/export images to Facebook web service
 *
 * Copyright (C) 2008-2009 by Luka Renko <lure at kubuntu dot org>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "fbtalker.h"

// C++ includes
#include <ctime>

// Qt includes
#include <QByteArray>
#include <QDomDocument>
#include <QDomElement>
#include <QtAlgorithms>

// KDE includes
#include <KCodecs>
#include <KDebug>
#include <KIO/Job>
#include <KIO/JobUiDelegate>
#include <KApplication>
#include <KMessageBox>
#include <KToolInvocation>
#include <KLocale>

// Local includes
#include "fbitem.h"
#include "mpform.h"

static QString kipiplugins_version="kamoso";

namespace KIPIFacebookPlugin
{

bool operator< (const FbUser& first, const FbUser& second)
{
    return first.name < second.name;
}

bool operator< (const FbAlbum& first, const FbAlbum& second)
{
    return first.title < second.title;
}

FbTalker::FbTalker(QWidget* parent)
{
    m_parent = parent;
    m_job    = 0;

    m_userAgent  = QString("KIPI-Plugin-Fb/%1 (lure@kubuntu.org)").arg(kipiplugins_version);
    m_apiVersion = "1.0";
    m_apiURL     = "https://api.facebook.com/restserver.php";
    m_apiKey     = "bf430ad869b88aba5c0c17ea6707022b";
    m_secretKey  = "0434307e70dd12c414cc6d0928f132d8";
}

FbTalker::~FbTalker()
{
    // do not logout - may reuse session for next upload

    if (m_job)
        m_job->kill();
}

bool FbTalker::loggedIn() const
{
    return !m_sessionKey.isEmpty();
}

QString FbTalker::getSessionKey() const
{
    return m_sessionKey;
}

QString FbTalker::getSessionSecret() const
{
    return m_sessionSecret;
}

unsigned int FbTalker::getSessionExpires() const
{
    return m_sessionExpires;
}

FbUser FbTalker::getUser() const
{
    return m_user;
}

void FbTalker::cancel()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }

    emit signalBusy(false);
}

/** Compute MD5 signature using url queries keys and values:
    http://wiki.developers.facebook.com/index.php/How_Facebook_Authenticates_Your_Application
*/
QString FbTalker::getApiSig(const QMap<QString, QString>& args)
{
    QString concat;
    // NOTE: QMap iterator will sort alphabetically
    for (QMap<QString, QString>::const_iterator it = args.constBegin();
         it != args.constEnd();
         ++it)
    {
        concat.append(it.key());
        concat.append("=");
        concat.append(it.value());
    }
    if (args["session_key"].isEmpty())
        concat.append(m_secretKey);
    else
        concat.append(m_sessionSecret);

    KMD5 md5(concat.toUtf8());
    return md5.hexDigest().data();
}

QString FbTalker::getCallString(const QMap<QString, QString>& args)
{
    QString concat;
    // NOTE: QMap iterator will sort alphabetically
    for (QMap<QString, QString>::const_iterator it = args.constBegin();
         it != args.constEnd();
         ++it)
    {
        if (!concat.isEmpty())
            concat.append("&");
        concat.append(it.key());
        concat.append("=");
        concat.append(it.value());
    }

    kDebug(51000) << "CALL: " << concat;

    return concat;
}

void FbTalker::authenticate(const QString &sessionKey,
                            const QString &sessionSecret,
                            unsigned int sessionExpires)
{
    m_loginInProgress = true;

    if (!sessionKey.isEmpty()
        && !sessionSecret.isEmpty()
        && sessionExpires > (unsigned int)(time(0) + 900))
    {
        // sessionKey seems to be still valid for at least 15 minutes
        // - check if it still works
        m_sessionKey     = sessionKey;
        m_sessionSecret  = sessionSecret;
        m_sessionExpires = sessionExpires;

        emit signalLoginProgress(1, 8, i18n("Validate previous session..."));

        // get logged in user - this will check if session is still valid
        getLoggedInUser();
    }
    else
    {
        // session expired -> get new authorization token and session
        createToken();
    }
}

void FbTalker::createToken()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);
    emit signalLoginProgress(1, 8, i18n("Logging in to Facebook service..."));

    QMap<QString, QString> args;
    args["method"]  = "facebook.auth.createToken";
    args["api_key"] = m_apiKey;
    args["v"]       = m_apiVersion;
    args["sig"]     = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_CREATETOKEN;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::getSession()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);
    emit signalLoginProgress(3);

    QMap<QString, QString> args;
    args["method"]      = "facebook.auth.getSession";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["auth_token"]  = m_authToken;
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_GETSESSION;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::getLoggedInUser()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);
    emit signalLoginProgress(2);

    QMap<QString, QString> args;
    args["method"]      = "facebook.users.getLoggedInUser";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["call_id"]     = QString::number(m_callID.elapsed());
    args["session_key"] = m_sessionKey;
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_GETLOGGEDINUSER;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::getUserInfo(const QString& userIDs)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    if (userIDs.isEmpty()) {
        emit signalBusy(true);
        emit signalLoginProgress(5);
    }

    QMap<QString, QString> args;
    args["method"]      = "facebook.users.getInfo";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["call_id"]     = QString::number(m_callID.elapsed());
    args["session_key"] = m_sessionKey;
    if (userIDs.isEmpty())
        args["uids"]    = QString::number(m_user.id);
    else
        args["uids"]    = userIDs;
    args["fields"]      = "name,profile_url";
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    if (userIDs.isEmpty())
        m_state = FB_GETUSERINFO;
    else
        m_state = FB_GETUSERINFO_FRIENDS;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::getUploadPermission()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);
    if (m_loginInProgress)
        emit signalLoginProgress(7);

    QMap<QString, QString> args;
    args["method"]      = "facebook.users.hasAppPermission";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["call_id"]     = QString::number(m_callID.elapsed());
    args["session_key"] = m_sessionKey;
    args["ext_perm"]    = "photo_upload";
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_GETUPLOADPERM;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::changePerm()
{
    m_loginInProgress = false; // just in case

    emit signalBusy(true);

    KUrl url("https://www.facebook.com/authorize.php");
    url.addQueryItem("api_key", m_apiKey);
    url.addQueryItem("v", m_apiVersion);
    url.addQueryItem("ext_perm", "photo_upload");
    kDebug(51000) << "Change Perm URL: " << url;
    KToolInvocation::invokeBrowser(url.url());

    emit signalBusy(false);
    KMessageBox::information(kapp->activeWindow(),
                  i18n("Please follow the instructions in the browser window. "
                       "Press \"OK\" when done."),
                  i18n("Facebook Application Authorization"));

    emit signalBusy(true);
    getUploadPermission();
}

void FbTalker::logout()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.auth.expireSession";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["session_key"] = m_sessionKey;
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    m_state = FB_LOGOUT;
    m_job   = job;
    m_buffer.resize(0);

    // logout is synchronous call
    job->exec();
    slotResult(job);
}

void FbTalker::listFriends()
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.friends.get";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["session_key"] = m_sessionKey;
    args["call_id"]     = QString::number(m_callID.elapsed());
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_LISTFRIENDS;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::listAlbums(long long userID)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.photos.getAlbums";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["session_key"] = m_sessionKey;
    args["call_id"]     = QString::number(m_callID.elapsed());
    if (userID != 0)
        args["uid"]     = QString::number(userID);
    else
        args["uid"]     = QString::number(m_user.id);
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_LISTALBUMS;
    m_job   = job;
    m_buffer.resize(0);
}


void FbTalker::listPhotos(long long userID, long long albumID)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.photos.get";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["session_key"] = m_sessionKey;
    args["call_id"]     = QString::number(m_callID.elapsed());
    if (albumID != 0)
        args["aid"]     = QString::number(albumID);
    else if (userID != 0)
        args["subj_id"] = QString::number(userID);
    else
        args["subj_id"] = QString::number(m_user.id);
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_LISTPHOTOS;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::createAlbum(const FbAlbum& album)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.photos.createAlbum";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["session_key"] = m_sessionKey;
    args["name"]        = album.title;
    if (!album.location.isEmpty())
        args["location"] = album.location;
    if (!album.description.isEmpty())
        args["description"] = album.description;
    switch (album.privacy)
    {
        case FB_FRIENDS:
            args["visible"] = "friends";
            break;
        case FB_FRIENDS_OF_FRIENDS:
            args["visible"] = "friends-of-friends";
            break;
        case FB_NETWORKS:
            args["visible"] = "networks";
            break;
        case FB_EVERYONE:
            args["visible"] = "everyone";
            break;
    }
    args["sig"]         = getApiSig(args);

    QByteArray tmp(getCallString(args).toUtf8());
    KIO::TransferJob* job = KIO::http_post(m_apiURL, tmp, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type",
                     "Content-Type: application/x-www-form-urlencoded");

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_CREATEALBUM;
    m_job   = job;
    m_buffer.resize(0);
}

bool FbTalker::addPhoto(const QString& imgPath,
                        long long albumID,
                        const QString& caption)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    QMap<QString, QString> args;
    args["method"]      = "facebook.photos.upload";
    args["api_key"]     = m_apiKey;
    args["v"]           = m_apiVersion;
    args["call_id"]     = QString::number(m_callID.elapsed());
    args["session_key"] = m_sessionKey;
    args["name"]        = KUrl(imgPath).fileName();
    if (albumID > 0)
        args["aid"]     = QString::number(albumID);
    if (!caption.isEmpty())
        args["caption"] = caption;
    args["sig"]         = getApiSig(args);

    MPForm  form;
    for (QMap<QString, QString>::const_iterator it = args.constBegin();
         it != args.constEnd();
         ++it)
    {
        form.addPair(it.key(), it.value());
    }

    if (!form.addFile(args["name"], imgPath))
    {
        emit signalBusy(false);
        return false;
    }
    form.finish();


    kDebug(51000) << "FORM: " << endl << form.formData();

    KIO::TransferJob* job = KIO::http_post(m_apiURL, form.formData(), KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);
    job->addMetaData("content-type", form.contentType());

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob *)),
            this, SLOT(slotResult(KJob *)));

    m_state = FB_ADDPHOTO;
    m_job   = job;
    m_buffer.resize(0);
    return true;
}

void FbTalker::getPhoto(const QString& imgPath)
{
    if (m_job)
    {
        m_job->kill();
        m_job = 0;
    }
    emit signalBusy(true);

    KIO::TransferJob* job = KIO::get(imgPath, KIO::Reload, KIO::HideProgressInfo);
    job->addMetaData("UserAgent", m_userAgent);

    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(data(KIO::Job*, const QByteArray&)));

    connect(job, SIGNAL(result(KJob*)),
            this, SLOT(slotResult(KJob*)));

    m_state = FB_GETPHOTO;
    m_job   = job;
    m_buffer.resize(0);
}

void FbTalker::data(KIO::Job*, const QByteArray& data)
{
    if (data.isEmpty())
        return;

    int oldSize = m_buffer.size();
    m_buffer.resize(m_buffer.size() + data.size());
    memcpy(m_buffer.data()+oldSize, data.data(), data.size());
}

QString FbTalker::errorToText(int errCode, const QString &errMsg)
{
    QString transError;
    kDebug(51000) << "errorToText: " << errCode << ": " << errMsg;

    switch (errCode)
    {
        case 0:
            transError = "";
            break;
        case 2:
            transError = i18n("The service is not available at this time.");
            break;
        case 4:
            transError = i18n("The application has reached the maximum number of requests allowed.");
            break;
        case 102:
            transError = i18n("Invalid session key or session expired. Try to log in again.");
            break;
        case 120:
            transError = i18n("Invalid album ID.");
            break;
        case 321:
            transError = i18n("Album is full.");
            break;
        case 324:
            transError = i18n("Missing or invalid file.");
            break;
        case 325:
            transError = i18n("Too many unapproved photos pending.");
            break;
        default:
            transError = errMsg;
            break;
    }
    return transError;
}

void FbTalker::slotResult(KJob *kjob)
{
    m_job = 0;
    KIO::Job *job = static_cast<KIO::Job*>(kjob);

    if (job->error())
    {
        if (m_loginInProgress)
        {
            authenticationDone(job->error(), job->errorText());
        }
        else if (m_state == FB_ADDPHOTO)
        {
            emit signalBusy(false);
            emit signalAddPhotoDone(job->error(), job->errorText());
        }
        else if (m_state == FB_GETPHOTO)
        {
            emit signalBusy(false);
            emit signalGetPhotoDone(job->error(), job->errorText(), QByteArray());
        }
        else
        {
            emit signalBusy(false);
            job->ui()->setWindow(m_parent);
            job->ui()->showErrorMessage();
        }
        return;
    }

    switch(m_state)
    {
        case(FB_CREATETOKEN):
            parseResponseCreateToken(m_buffer);
            break;
        case(FB_GETSESSION):
            parseResponseGetSession(m_buffer);
            break;
        case(FB_GETLOGGEDINUSER):
            parseResponseGetLoggedInUser(m_buffer);
            break;
        case(FB_GETUSERINFO):
        case(FB_GETUSERINFO_FRIENDS):
            parseResponseGetUserInfo(m_buffer);
            break;
        case(FB_GETUPLOADPERM):
            parseResponseGetUploadPermission(m_buffer);
            break;
        case(FB_LOGOUT):
            parseResponseLogout(m_buffer);
            break;
        case(FB_LISTFRIENDS):
            parseResponseListFriends(m_buffer);
            break;
        case(FB_LISTALBUMS):
            parseResponseListAlbums(m_buffer);
            break;
        case(FB_LISTPHOTOS):
            parseResponseListPhotos(m_buffer);
            break;
        case(FB_CREATEALBUM):
            parseResponseCreateAlbum(m_buffer);
            break;
        case(FB_ADDPHOTO):
            parseResponseAddPhoto(m_buffer);
            break;
        case(FB_GETPHOTO):
            // all we get is data of the image
            emit signalBusy(false);
            emit signalGetPhotoDone(0, QString(), m_buffer);
            break;
    }
}

void FbTalker::authenticationDone(int errCode, const QString &errMsg)
{
    if (errCode != 0)
    {
        m_authToken.clear();
        m_sessionKey.clear();
        m_sessionSecret.clear();
        m_sessionExpires = 0;
        m_user.clear();
    }

    emit signalBusy(false);
    emit signalLoginDone(errCode, errMsg);
    m_loginInProgress = false;
}

int FbTalker::parseErrorResponse(const QDomElement& e, QString& errMsg)
{
    int errCode = -1;

    for (QDomNode node = e.firstChild();
         !node.isNull();
         node = node.nextSibling())
    {
        if (!node.isElement())
            continue;
        if (node.nodeName() == "error_code")
        {
            errCode = node.toElement().text().toInt();
            kDebug(51000) << "Error Code:" << errCode;
        }
        else if (node.nodeName() == "error_msg")
        {
            errMsg = node.toElement().text();
            kDebug(51000) << "Error Text:" << errMsg;
        }
    }
    return errCode;
}

void FbTalker::parseResponseCreateToken(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("createToken");
    if (!doc.setContent(data))
        return;

    emit signalLoginProgress(2);

    kDebug(51000) << "Parse CreateToken response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "auth_createToken_response")
    {
        m_authToken = docElem.text();
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (errCode != 0) // if login failed, reset user properties
    {
        authenticationDone(errCode, errorToText(errCode, errMsg));
        return;
    }

    KUrl url("https://www.facebook.com/login.php");
    url.addQueryItem("api_key", m_apiKey);
    url.addQueryItem("v", m_apiVersion);
    url.addQueryItem("auth_token", m_authToken);
    kDebug(51000) << "Login URL: " << url;
    KToolInvocation::invokeBrowser(url.url());

    emit signalBusy(false);
    int valueOk = KMessageBox::questionYesNo(kapp->activeWindow(),
                  i18n("Please follow the instructions in the browser window. "
                       "Press \"Yes\" if you have authenticated and \"No\" if you failed."),
                  i18n("Facebook Service Web Authorization"));

    if (valueOk == KMessageBox::Yes)
    {
        emit signalBusy(true);
        getSession();
    }
    else
    {
        authenticationDone(-1, i18n("Canceled by user."));
    }
}

void FbTalker::parseResponseGetSession(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("getSession");
    if (!doc.setContent(data))
        return;

    emit signalLoginProgress(4);

    kDebug(51000) << "Parse GetSession response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "auth_getSession_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "session_key")
                m_sessionKey = node.toElement().text();
            else if (node.nodeName() == "secret")
                m_sessionSecret = node.toElement().text();
            else if (node.nodeName() == "uid")
                m_user.id = node.toElement().text().toLongLong();
            else if (node.nodeName() == "expires")
                m_sessionExpires = node.toElement().text().toInt();
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (errCode != 0) // if login failed, reset user properties
    {
        authenticationDone(errCode, errorToText(errCode, errMsg));
        return;
    }

    // reset call_id counter
    m_callID.start();

    getUserInfo();
}

void FbTalker::parseResponseGetLoggedInUser(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("getLoggedInUser");
    if (!doc.setContent(data))
        return;

    emit signalLoginProgress(3);

    kDebug(51000) << "Parse GetLoggedInUser response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "users_getLoggedInUser_response")
    {
        m_user.id = docElem.text().toLongLong();
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (errCode == 0)
    {
        // session is still valid -> get full user info
        getUserInfo();
    }
    else
    {
        // it seems that session expired -> create new token and session
        m_authToken.clear();
        m_sessionKey.clear();
        m_sessionSecret.clear();
        m_sessionExpires = 0;
        m_user.clear();

        createToken();
    }
}

void FbTalker::parseResponseGetUserInfo(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;
    QDomDocument doc("getUserInfo");
    if (!doc.setContent(data))
        return;

    if (m_state == FB_GETUSERINFO) // during login
        emit signalLoginProgress(6);

    kDebug(51000) << "Parse GetUserInfo response:" << endl << data;

    QList<FbUser> friendsList;
    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "users_getInfo_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "user")
            {
                FbUser user;
                for (QDomNode nodeU = node.toElement().firstChild();
                     !nodeU.isNull();
                     nodeU = nodeU.nextSibling())
                {
                    if (!nodeU.isElement())
                        continue;
                    if (nodeU.nodeName() == "uid")
                        user.id = nodeU.toElement().text().toLongLong();
                    else if (nodeU.nodeName() == "name")
                        user.name = nodeU.toElement().text();
                    else if (nodeU.nodeName() == "profile_url")
                        user.profileURL = nodeU.toElement().text();
                }
                if (m_state == FB_GETUSERINFO)
                {
                    m_user.name = user.name;
                    m_user.profileURL = user.profileURL;
                }
                else if (!user.name.isEmpty())
                    friendsList.append(user);
            }
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (m_state == FB_GETUSERINFO)
    {
        if (errCode != 0)
        {
            authenticationDone(errCode, errorToText(errCode, errMsg));
            return;
        }

        getUploadPermission();
    }
    else
    {
        qSort(friendsList.begin(), friendsList.end());

        emit signalBusy(false);
        emit signalListFriendsDone(errCode, errorToText(errCode, errMsg),
                                   friendsList);
    }
}

void FbTalker::parseResponseGetUploadPermission(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("getUploadPerm");
    if (!doc.setContent(data))
        return;

    if (m_loginInProgress)
        emit signalLoginProgress(8);

    kDebug(51000) << "Parse HasAppPermission response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "users_hasAppPermission_response")
    {
        m_user.uploadPerm = docElem.text().toInt();
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (m_loginInProgress)
        authenticationDone(errCode, errorToText(errCode, errMsg));
    else
    {
        emit signalBusy(false);
        emit signalChangePermDone(errCode, errorToText(errCode, errMsg));
    }
}

void FbTalker::parseResponseLogout(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("expireSession");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse ExpireSession response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "auth_expireSession_response ")
    {
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    // consider we are logged out in any case
    m_sessionKey.clear();
    m_sessionSecret.clear();
    m_sessionExpires = 0;
    m_user.clear();

    emit signalBusy(false);
}

void FbTalker::parseResponseAddPhoto(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;

    QDomDocument doc("addphoto");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse Add Photo response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "photos_upload_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    emit signalBusy(false);
    emit signalAddPhotoDone(errCode, errorToText(errCode, errMsg));
}

void FbTalker::parseResponseCreateAlbum(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;
    QDomDocument doc("createalbum");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse Create Album response:" << endl << data;

    long long newAlbumID = -1;
    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "photos_createAlbum_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "aid")
            {
                newAlbumID = node.toElement().text().toLongLong();
                kDebug(51000) << "newAID: " << newAlbumID;
            }
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    emit signalBusy(false);
    emit signalCreateAlbumDone(errCode, errorToText(errCode, errMsg),
                               newAlbumID);
}

void FbTalker::parseResponseListFriends(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;
    QDomDocument doc("getFriends");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse Friends response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    QString friendsUIDs;
    if (docElem.tagName() == "friends_get_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "uid")
            {
                if (!friendsUIDs.isEmpty())
                    friendsUIDs.append(',');
                friendsUIDs.append(node.toElement().text());
            }
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    if (friendsUIDs.isEmpty())
    {
        emit signalBusy(false);

        QList<FbUser> noFriends;
        emit signalListFriendsDone(errCode, errorToText(errCode, errMsg),
                                   noFriends);
    }
    else
    {
        // get user info for those users
        getUserInfo(friendsUIDs);
    }
}

void FbTalker::parseResponseListAlbums(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;
    QDomDocument doc("getAlbums");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse Albums response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    QList <FbAlbum> albumsList;
    if (docElem.tagName() == "photos_getAlbums_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "album")
            {
                FbAlbum album;
                for (QDomNode nodeA = node.toElement().firstChild();
                     !nodeA.isNull();
                     nodeA = nodeA.nextSibling())
                {
                    if (!nodeA.isElement())
                        continue;
                    if (nodeA.nodeName() == "aid")
                        album.id = nodeA.toElement().text().toLongLong();
                    else if (nodeA.nodeName() == "name")
                        album.title = nodeA.toElement().text();
                    else if (nodeA.nodeName() == "description")
                        album.description = nodeA.toElement().text();
                    else if (nodeA.nodeName() == "location")
                        album.location = nodeA.toElement().text();
                    else if (nodeA.nodeName() == "link")
                        album.url = nodeA.toElement().text();
                    else if (nodeA.nodeName() == "visible")
                    {
                        if (nodeA.toElement().text() == "friends")
                            album.privacy = FB_FRIENDS;
                        else if (nodeA.toElement().text() == "friends-of-friends")
                            album.privacy = FB_FRIENDS_OF_FRIENDS;
                        else if (nodeA.toElement().text() == "networks")
                            album.privacy = FB_NETWORKS;
                        else if (nodeA.toElement().text() == "everyone")
                            album.privacy = FB_EVERYONE;
                    }
                }
                kDebug(51000) << "AID: " << album.id;
                albumsList.append(album);
            }
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    qSort(albumsList.begin(), albumsList.end());

    emit signalBusy(false);
    emit signalListAlbumsDone(errCode, errorToText(errCode, errMsg),
                              albumsList);
}

void FbTalker::parseResponseListPhotos(const QByteArray& data)
{
    int errCode = -1;
    QString errMsg;
    QDomDocument doc("getPhotos");
    if (!doc.setContent(data))
        return;

    kDebug(51000) << "Parse Photos response:" << endl << data;

    QDomElement docElem = doc.documentElement();
    QList <FbPhoto> photosList;
    if (docElem.tagName() == "photos_get_response")
    {
        for (QDomNode node = docElem.firstChild();
             !node.isNull();
             node = node.nextSibling())
        {
            if (!node.isElement())
                continue;
            if (node.nodeName() == "photo")
            {
                FbPhoto photo;
                for (QDomNode nodeP = node.toElement().firstChild();
                     !nodeP.isNull();
                     nodeP = nodeP.nextSibling())
                {
                    if (!nodeP.isElement())
                        continue;
                    if (nodeP.nodeName() == "pid")
                        photo.id = nodeP.toElement().text().toLongLong();
                    else if (nodeP.nodeName() == "caption")
                        photo.caption = nodeP.toElement().text();
                    else if (nodeP.nodeName() == "src_small")
                        photo.thumbURL = nodeP.toElement().text();
                    else if (nodeP.nodeName() == "src_big")
                        photo.originalURL = nodeP.toElement().text();
                    else if (nodeP.nodeName() == "src"
                             && photo.originalURL.isEmpty())
                        photo.originalURL = nodeP.toElement().text();
                }
                photosList.append(photo);
            }
        }
        errCode = 0;
    }
    else if (docElem.tagName() == "error_response")
        errCode = parseErrorResponse(docElem, errMsg);

    emit signalBusy(false);
    emit signalListPhotosDone(errCode, errorToText(errCode, errMsg),
                              photosList);
}

} // namespace KIPIFacebookPlugin
