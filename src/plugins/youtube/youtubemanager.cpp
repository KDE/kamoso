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

#include "youtubemanager.h"
#include <QDebug>
#include <KIO/Job>
#include <KUrl>

YoutubeManager::YoutubeManager(QByteArray username,QByteArray password, QByteArray developerKey)
{
	setUsername(username);
	setPassword(password);
	setDeveloperKey(developerKey);
}

void YoutubeManager::login()
{
	KUrl url("https://www.google.com/youtube/accounts/ClientLogin");
	QByteArray data("Email=tetasnor&Passwd=12344321&service=youtube&source=Kamoso");
	KIO::TransferJob *loginJob = KIO::http_post(url,data,KIO::HideProgressInfo);
	loginJob->addMetaData("cookies","none");
	loginJob->addMetaData("content-type","Content-Type:application/x-www-form-urlencoded");
	connect(loginJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(loginDone(KIO::Job *, const QByteArray &)));
	loginJob->start();
}

void YoutubeManager::upload(QByteArray *uploadPath)
{
	qDebug() << "File To Upload: " << uploadPath->data();
	KUrl url(*uploadPath);
	KIO::TransferJob *getFileContentJob = KIO::get(url,KIO::NoReload,KIO::HideProgressInfo);
	connect(getFileContentJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(fileOpened(KIO::Job *, const QByteArray &)));
	getFileContentJob->start();
}

void YoutubeManager::fileOpened(KIO::Job *job, const QByteArray &data)
{
	delete job;
	QByteArray extraHeaders("");
	extraHeaders.append("Authorization: GoogleLogin auth=");
	extraHeaders.append(m_authToken.data());
	extraHeaders.append("\r\n");
	extraHeaders.append("GData-Version: 2");
	extraHeaders.append("\r\n");
	extraHeaders.append("X-GData-Key: key=");
	extraHeaders.append(developerKey().data());
	extraHeaders.append("\r\n");
	extraHeaders.append("Slug: kamoso_06332009_053348.ogv");

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
    finalData.append("<media:title type=\"plain\">Bad Wedding Toast</media:title>\r\n");
    finalData.append("<media:description type=\"plain\">\r\n");
      finalData.append("I gave a bad toast at my friend's wedding.\r\n");
    finalData.append("</media:description>\r\n");
    finalData.append("<media:category\r\n");
      finalData.append("scheme=\"http://gdata.youtube.com/schemas/2007/categories.cat\">People\r\n");
    finalData.append("</media:category>\r\n");
    finalData.append("<media:keywords>toast, wedding</media:keywords>\r\n");
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
	finalData.append("\r\n");
	finalData.append("--foobarfoo--");


	KUrl url("http://uploads.gdata.youtube.com/feeds/api/users/default/uploads");
	KIO::TransferJob *uploadJob = KIO::http_post(url,finalData,KIO::HideProgressInfo);
	uploadJob->addMetaData("cookies","none");
	uploadJob->addMetaData("connection","close");
	uploadJob->addMetaData("customHTTPHeader",extraHeaders.data());
	uploadJob->addMetaData("content-type","Content-Type: multipart/related; boundary=\"foobarfoo\"");
	connect(uploadJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(uploadDone(KIO::Job *, const QByteArray &)));
	uploadJob->start();
}

void YoutubeManager::uploadDone(KIO::Job *job, const QByteArray &data)
{
	qDebug() << "Upload Response" << data.data();
}

void YoutubeManager::loginDone(KIO::Job *job, const QByteArray &data)
{
	delete job;
	#warning do we have to disconnect before delete the job?
// 	disconnect(SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(loginDone(KIO::Job *, const QByteArray &)));
	qDebug() << "LoginDone, data received\n";
	qDebug() << data.data();
	if(data.at(0) == 'E'){
		emit authenticated(false);
	}else{
		QList<QByteArray> tokens = data.split('\n');
		setAuthToken(tokens.first().remove(0,5));
		qDebug() << "Final AuthToken: " << m_authToken.data();
		#warning I've to use the setter but I've some problems due to convertion, cast it!
		emit authenticated(true);
	}
}
void YoutubeManager::setUsername(QByteArray username)
{
	m_username = username;
}

void YoutubeManager::setPassword(QByteArray password)
{
	m_password = password;
}

void YoutubeManager::setDeveloperKey(QByteArray developerKey)
{
	m_developerKey = developerKey;
}

void YoutubeManager::setAuthToken(QByteArray authToken)
{
	m_authToken = authToken;
}

QByteArray YoutubeManager::username() const
{
	return m_username;
}

QByteArray YoutubeManager::password() const
{
	return m_password;
}

QByteArray YoutubeManager::developerKey() const
{
	return m_developerKey;
}

QByteArray YoutubeManager::authToken() const
{
	return m_authToken;
}