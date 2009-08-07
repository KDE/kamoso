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

#ifndef FBTALKER_H
#define FBTALKER_H

// Qt includes
#include <QList>
#include <QString>
#include <QTime>
#include <QObject>

// KDE includes
#include <KIO/Job>

// local includes
#include "fbitem.h"

class QDomElement;

namespace KIPIFacebookPlugin
{

class FbTalker : public QObject
{
    Q_OBJECT

public:
    FbTalker(QWidget* parent);
    ~FbTalker();

    QString         getSessionKey() const;
    QString         getSessionSecret() const;
    unsigned int    getSessionExpires() const;

    FbUser  getUser() const;

    bool    loggedIn() const;
    void    cancel();
    void    authenticate(const QString& sessionKey, 
                         const QString& sessionSecret,
                         unsigned int sessionExpires);
    void    changePerm();
    void    logout();

    void    listFriends();

    void    listAlbums(long long userID = 0);
    void    listPhotos(long long userID, long long albumID);

    void    createAlbum(const FbAlbum& album);

    bool    addPhoto(const QString& imgPath, long long albumID, 
                     const QString& caption);
    void    getPhoto(const QString& imgPath);

signals:
    void signalBusy(bool val);
    void signalLoginProgress(int step, int maxStep = 0, 
                             const QString& label = "");
    void signalLoginDone(int errCode, const QString& errMsg);
    void signalChangePermDone(int errCode, const QString& errMsg);
    void signalAddPhotoDone(int errCode, const QString& errMsg);
    void signalGetPhotoDone(int errCode, const QString& errMsg,
                            const QByteArray& photoData);
    void signalCreateAlbumDone(int errCode, const QString& errMsg,
                               long long newAlbumID);
    void signalListAlbumsDone(int errCode, const QString& errMsg,
                              const QList <KIPIFacebookPlugin::FbAlbum>& albumsList);
    void signalListPhotosDone(int errCode, const QString& errMsg,
                              const QList <KIPIFacebookPlugin::FbPhoto>& photosList);
    void signalListFriendsDone(int errCode, const QString& errMsg,
                               const QList <KIPIFacebookPlugin::FbUser>& friendsList);

private:
    enum State
    {
        FB_CREATETOKEN = 0,
        FB_GETSESSION,
        FB_GETLOGGEDINUSER,
        FB_GETUSERINFO,
        FB_GETUSERINFO_FRIENDS,
        FB_GETUPLOADPERM,
        FB_LOGOUT,
        FB_LISTFRIENDS,
        FB_LISTALBUMS,
        FB_LISTPHOTOS,
        FB_CREATEALBUM,
        FB_ADDPHOTO,
        FB_GETPHOTO
    };

    QString getApiSig(const QMap<QString, QString>& args);
    QString getCallString(const QMap<QString, QString>& args);
    void    authenticationDone(int errCode, const QString& errMsg);
    void    createToken();
    void    getSession();
    void    getLoggedInUser();
    void    getUserInfo(const QString& userIDs = "");
    void    getUploadPermission();

    QString errorToText(int errCode, const QString& errMsg);
    int parseErrorResponse(const QDomElement& e, QString& errMsg);
    void parseResponseCreateToken(const QByteArray& data);
    void parseResponseGetSession(const QByteArray& data);
    void parseResponseGetLoggedInUser(const QByteArray& data);
    void parseResponseGetUserInfo(const QByteArray& data);
    void parseResponseGetUploadPermission(const QByteArray& data);
    void parseResponseLogout(const QByteArray& data);
    void parseResponseAddPhoto(const QByteArray& data);
    void parseResponseCreateAlbum(const QByteArray& data);
    void parseResponseListAlbums(const QByteArray& data);
    void parseResponseListPhotos(const QByteArray& data);
    void parseResponseListFriends(const QByteArray& data);

private slots:
    void data(KIO::Job *job, const QByteArray& data);
    void slotResult(KJob *job);

private:
    QWidget*        m_parent;

    QByteArray      m_buffer;

    QString         m_userAgent;
    QString         m_apiURL;
    QString         m_apiVersion;
    QString         m_apiKey;
    QString         m_secretKey;

    bool            m_loginInProgress;
    QString         m_authToken;
    QString         m_sessionKey;
    QString         m_sessionSecret;
    unsigned int    m_sessionExpires;
    QTime           m_callID;

    FbUser          m_user;

    KIO::Job*       m_job;

    State           m_state;
};

} // namespace KIPIFacebookPlugin

#endif /* FBTALKER_H */
