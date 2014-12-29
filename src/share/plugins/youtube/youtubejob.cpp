/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <afiestas@kde.org>                       *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include "youtubejob.h"
#include <kpassworddialog.h>

#include <QApplication>

#include <QDebug>
#include <QUrl>
#include <QIcon>
#include <QDesktopServices>
#include <KIO/Job>
#include <KToolInvocation>
#include <KLocalizedString>

const QByteArray YoutubeJob::developerKey("AI39si41ZFrIJoZGNH0hrZPhMuUlwHc6boMLi4e-_W6elIzVUIeDO9F7ix2swtnGAiKT4yc4F4gQw6yysTGvCn1lPNyli913Xg");

using KWallet::Wallet;
YoutubeJob::YoutubeJob(const QUrl& url, const QString& title, const QString& tags, const QString& description, QObject* parent)
    : KJob(parent), m_authToken(0), m_url(url), m_title(title), m_tags(tags), m_description(description), dialog(0)
{
}

void YoutubeJob::start()
{
    qDebug() << "Job started";
    checkWallet();
}

void YoutubeJob::checkWallet()
{
    WId windowId = 0;
    if (qApp->activeWindow()) {
        windowId = qApp->activeWindow()->winId();
    }
    m_wallet = Wallet::openWallet(Wallet::NetworkWallet(), windowId);
    if(m_wallet != NULL){
        if(!m_wallet->hasFolder("youtubeKamoso")){
            if(!m_wallet->createFolder("youtubeKamoso")){
                //TODO: Error reporting here
                return;
            }
        }
        m_wallet->setFolder("youtubeKamoso");
    }

    if(!showDialog()){
        emit emitResult();
        return;
    }
    login();
}

void YoutubeJob::fileOpened(KIO::Job *job, const QByteArray &data)
{
    qDebug() << "fileOPened!!";
    job->suspend();
    #warning do something to evade the cast? like adding a metadata?
    KIO::SimpleJob *simpleJob = static_cast<KIO::SimpleJob*>(job);

    disconnect(job,SIGNAL(data(KIO::Job*,QByteArray)),this,SLOT(fileOpened(KIO::Job*,QByteArray)));
    connect(job,SIGNAL(data(KIO::Job*,QByteArray)),this,SLOT(moreData(KIO::Job*,QByteArray)));

    QByteArray extraHeaders("");
    extraHeaders.append("Authorization: GoogleLogin auth=");
    extraHeaders.append(m_authToken.data());
    extraHeaders.append("\r\n");
    extraHeaders.append("GData-Version: 2");
    extraHeaders.append("\r\n");
    extraHeaders.append("X-GData-Key: key=");
    extraHeaders.append(developerKey);
    extraHeaders.append("\r\n");
    extraHeaders.append("Slug: ");
    extraHeaders.append(simpleJob->url().fileName());

    QByteArray finalData("--foobarfoo");
    finalData.append("\r\n");
    finalData.append("Content-Type: application/atom+xml; charset=UTF-8");
    finalData.append("\r\n");
    finalData.append("\r\n");
    finalData.append("<?xml version=\"1.0\"?>\r\n");
finalData.append("<entry xmlns=\"http://www.w3.org/2005/Atom\"\r\n");
  finalData.append("xmlns:media=\"http://search.yahoo.com/mrss/\"\r\n");
  finalData.append("xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\r\n");
  finalData.append("<media:group>\r\n");
    finalData.append("<media:title type=\"plain\">"+m_title+"</media:title>\r\n");
    finalData.append("<media:description type=\"plain\">\r\n");
      finalData.append(m_description+"\r\n");
    finalData.append("</media:description>\r\n");
    finalData.append("<media:category\r\n");
      finalData.append("scheme=\"http://gdata.youtube.com/schemas/2007/categories.cat\">People\r\n");
    finalData.append("</media:category>\r\n");
    finalData.append("<media:keywords>"+m_tags+"</media:keywords>\r\n");
  finalData.append("</media:group>\r\n");
finalData.append("</entry>");
    finalData.append("\r\n");
    finalData.append("--foobarfoo");
    finalData.append("\r\n");
    finalData.append("Content-Type: video/ogg");
    finalData.append("\r\n");
    finalData.append("Content-Transfer-Encoding: binary");
    finalData.append("\r\n");
    finalData.append("\r\n");
    finalData.append(data);
//     qDebug() << finalData;
    QUrl url("http://uploads.gdata.youtube.com/feeds/api/users/default/uploads");
    uploadJob = KIO::http_post(url,finalData,KIO::HideProgressInfo);
    uploadJob->addMetaData("cookies","none");
    uploadJob->addMetaData("connection","close");
    uploadJob->addMetaData("customHTTPHeader",extraHeaders.data());
    uploadJob->addMetaData("content-type","Content-Type: multipart/related; boundary=\"foobarfoo\"");
    uploadJob->setAsyncDataEnabled(true);
    connect(uploadJob,SIGNAL(dataReq(KIO::Job*,QByteArray&)),this,SLOT(uploadNeedData()));
    connect(uploadJob,SIGNAL(data(KIO::Job*,QByteArray)),this,SLOT(uploadDone(KIO::Job*,QByteArray)));
    uploadJob->start();
}

void YoutubeJob::moreData(KIO::Job *job, const QByteArray &data)
{
    job->suspend();
    if(data.size() == 0){
        qDebug() << "Data is zero, going to end this!";
        disconnect(uploadJob,SIGNAL(dataReq(KIO::Job*,QByteArray&)),this,SLOT(uploadNeedData()));
        connect(uploadJob,SIGNAL(dataReq(KIO::Job*,QByteArray&)),this,SLOT(uploadFinal()));

        QByteArray final("\r\n");
        final.append("--foobarfoo--");
        uploadJob->sendAsyncData(final);
    }else{
        qDebug() << "Sending more data....";
//         qDebug() << data;
        uploadJob->sendAsyncData(data);
    }
}

void YoutubeJob::uploadFinal()
{
    //Sending an empty QByteArray the job ends
    qDebug() << "Sendind the empty packed";
    uploadJob->sendAsyncData(QByteArray());
}

void YoutubeJob::uploadNeedData()
{
    qDebug() << "openFile job resumed!";
    openFileJob->resume();
}

void YoutubeJob::uploadDone(KIO::Job *job, const QByteArray &data)
{
    qDebug() << "Upload Response";
//     qDebug() << data.data();
    QString dataStr(data);
    QRegExp rx("<media:player url='(\\S+)'/>");
    dataStr.contains(rx);
//     qDebug() << rx.cap(1);
    QUrl url = rx.cap(1);
    if (!url.isEmpty()) {
        qDebug() << "Url : " << url.url();
        job->kill();
        QDesktopServices::openUrl(url.url());
        emit emitResult();
    }
}

void YoutubeJob::login()
{
    QMap<QString, QString> authInfo;
    authInfo["username"] = dialog->username();
    authInfo["password"] = dialog->password();

    QUrl url("https://www.google.com/youtube/accounts/ClientLogin");
    QByteArray data("Email=");
    data.append(authInfo["username"].toLatin1());
    data.append("&Passwd=");
    data.append(authInfo["password"].toLatin1());
    data.append("&service=youtube&source=Kamoso");
    KIO::TransferJob *loginJob = KIO::http_post(url,data,KIO::HideProgressInfo);
    loginJob->addMetaData("cookies","none");
    loginJob->addMetaData("content-type","Content-Type:application/x-www-form-urlencoded");
    connect(loginJob,SIGNAL(data(KIO::Job*,QByteArray)),this,SLOT(loginDone(KIO::Job*,QByteArray)));
    loginJob->start();
}

void YoutubeJob::loginDone(KIO::Job *job, const QByteArray &data)
{
    delete job;
    qDebug() << "LoginDone, data received\n";
//     qDebug() << data.data();
    if(data.at(0) == 'E'){
        authenticated(false);
    }else{
        QList<QByteArray> tokens = data.split('\n');
        m_authToken = tokens.first().remove(0,5);
        qDebug() << "Final AuthToken: " << m_authToken.data();
        authenticated(true);
    }
}

bool YoutubeJob::showDialog()
{
    QString server = QString("http://www.youtube.com");

    if(m_wallet != NULL) {
        dialog = new KPasswordDialog(0L,KPasswordDialog::ShowKeepPassword | KPasswordDialog::ShowUsernameLine);
        QMap<QString, QString> authInfo;
        m_wallet->readMap("youtubeAuth",authInfo);
        dialog->setPassword(authInfo["password"]);
        dialog->setUsername(authInfo["username"]);
        dialog->setKeepPassword(true);
    }else{
        dialog = new KPasswordDialog(0L,KPasswordDialog::ShowUsernameLine);
    }
    dialog->setPrompt(i18n("You need to supply a username and a password to be able to upload videos to YouTube"));
    dialog->addCommentLine(i18n("Server:"),server);
    dialog->setWindowTitle(i18n("Authentication for YouTube"));

    int response = dialog->exec();
    if(response == QDialog::Rejected){
        return false;
    }
    while((dialog->username() == "" || dialog->password() == "") && response == QDialog::Accepted )
    {
        response = dialog->exec();
        if(response == QDialog::Rejected){
            return false;
        }
    }

    if(dialog->keepPassword() == true &&  m_wallet != NULL) {
        QMap<QString, QString> toSave;
        toSave["username"] = dialog->username();
        toSave["password"] = dialog->password();
        m_wallet->writeMap("youtubeAuth",toSave);
        m_wallet->sync();
    }
    return true;
}

void YoutubeJob::authenticated(bool auth)
{
    qDebug() << "Authentication: " << auth ;
    if(auth == false){
        if(showDialog()){
            login();
        }
        return;
    }
    QMap<QString, QString> videoInfo;
//     m_videoInfo = data();
#warning todo
    qDebug() << "File To Upload: " << m_url.path();
    openFileJob = KIO::get(m_url,KIO::NoReload,KIO::HideProgressInfo);
    connect(openFileJob,SIGNAL(data(KIO::Job*,QByteArray)),this,SLOT(fileOpened(KIO::Job*,QByteArray)));
    openFileJob->start();
}
