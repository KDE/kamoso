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
	QByteArray data("Email=");
	data.append(username());
	data.append("&Passwd=");
	data.append(password());
	data.append("&service=youtube&source=Kamoso");
	KIO::TransferJob *loginJob = KIO::http_post(url,data,KIO::HideProgressInfo);
	loginJob->addMetaData("cookies","none");
	loginJob->addMetaData("content-type","Content-Type:application/x-www-form-urlencoded");
	connect(loginJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(loginDone(KIO::Job *, const QByteArray &)));
	loginJob->start();
}

void YoutubeManager::upload(const KUrl &url)
{
	qDebug() << "File To Upload: " << url.path();
// 	KIO::TransferJob *getFileContentJob = KIO::get(url,KIO::NoReload,KIO::HideProgressInfo);

	openFileJob = KIO::get(url,KIO::NoReload,KIO::HideProgressInfo);
	connect(openFileJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(fileOpened(KIO::Job *, const QByteArray &)));
	openFileJob->start();
}

void YoutubeManager::fileOpened(KIO::Job *job, const QByteArray &data)
{
	qDebug() << "fileOPened!!";
	job->suspend();
	#warning do something to evade the cast? like adding a metadata?
	KIO::SimpleJob *simpleJob = static_cast<KIO::SimpleJob*>(job);
	
	disconnect(job,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(fileOpened(KIO::Job *, const QByteArray &)));
	connect(job,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(moreData(KIO::Job *, const QByteArray &)));

	QByteArray extraHeaders("");
	extraHeaders.append("Authorization: GoogleLogin auth=");
	extraHeaders.append(m_authToken.data());
	extraHeaders.append("\r\n");
	extraHeaders.append("GData-Version: 2");
	extraHeaders.append("\r\n");
	extraHeaders.append("X-GData-Key: key=");
	extraHeaders.append(developerKey().data());
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

	KUrl url("http://uploads.gdata.youtube.com/feeds/api/users/default/uploads");
	uploadJob = KIO::http_post(url,finalData,KIO::HideProgressInfo);
	uploadJob->addMetaData("cookies","none");
	uploadJob->addMetaData("connection","close");
	uploadJob->addMetaData("customHTTPHeader",extraHeaders.data());
	uploadJob->addMetaData("content-type","Content-Type: multipart/related; boundary=\"foobarfoo\"");
	uploadJob->setAsyncDataEnabled(true);
	connect(uploadJob,SIGNAL(dataReq(KIO::Job*, QByteArray &)),this,SLOT(uploadNeedData()));
	connect(uploadJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(uploadDone(KIO::Job *, const QByteArray &)));
	uploadJob->start();
}
void YoutubeManager::moreData(KIO::Job *job, const QByteArray &data)
{
	job->suspend();
	if(data.size() == 0){
		qDebug() << "Data is zero, going to end this!";
		disconnect(uploadJob,SIGNAL(dataReq(KIO::Job*, QByteArray &)),this,SLOT(uploadNeedData()));
		connect(uploadJob,SIGNAL(dataReq(KIO::Job*, QByteArray &)),this,SLOT(uploadFinal()));

		QByteArray final("\r\n");
		final.append("--foobarfoo--");
		uploadJob->sendAsyncData(final);
	}else{
		qDebug() << "Sending more data....";
		uploadJob->sendAsyncData(data);
	}
}

void YoutubeManager::uploadFinal()
{
	//Sending an empty QByteArray the job ends
	qDebug() << "Sendind the empty packed";
	uploadJob->sendAsyncData(QByteArray());
}

void YoutubeManager::uploadNeedData()
{
	qDebug() << "openFile job resumed!";
	openFileJob->resume();
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