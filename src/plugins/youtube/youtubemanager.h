/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@gmail.com>                        *
 *  Copyright (C) 2008-2009 by Alex Fiestas <alex@eyeos.org>                         *
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

#ifndef YOUTUBEMANAGER_H
#define YOUTUBEMANAGER_H
#include <qbytearray.h>
#include <QObject>
#include <KIO/Job>

class YoutubeManager : public QObject
{
	Q_OBJECT
	public:
		YoutubeManager(QByteArray username,QByteArray password, QByteArray developerKey);
		void login();
		void upload(const KUrl &path);

		//Getters
		QByteArray username() const;
		QByteArray password() const;
		QByteArray developerKey() const;
		QByteArray authToken() const;
		//Setters
		void setUsername(QByteArray username);
		void setPassword(QByteArray password);
		void setDeveloperKey(QByteArray developerKey);
		void setAuthToken(QByteArray authToken);
	public slots:
		void loginDone(KIO::Job *, const QByteArray &);
		void fileOpened(KIO::Job *, const QByteArray &);
		void uploadDone(KIO::Job *, const QByteArray &);
		void moreData(KIO::Job *, const QByteArray &);
		void uploadNeedData();
		void uploadFinal();
	private:
		QByteArray m_username;
		QByteArray m_password;
		QByteArray m_developerKey;
		QByteArray m_authToken;
		KIO::TransferJob *openFileJob;
		KIO::TransferJob *uploadJob;
	signals:
		void authenticated(bool);
		void uploadDone(bool);
};

#endif // YOUTUBEMANAGER_H
