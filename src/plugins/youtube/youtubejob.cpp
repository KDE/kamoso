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

#include "youtubejob.h"
#include <QDebug>
#include <KIO/Job>
#include <KUrl>
#include <KIcon>
#include <KToolInvocation>

#define YOUTUBE_DEVELOPER_KEY "AI39si41ZFrIJoZGNH0hrZPhMuUlwHc6boMLi4e-_W6elIzVUIeDO9F7ix2swtnGAiKT4yc4F4gQw6yysTGvCn1lPNyli913Xg"
#include <KLocalizedString>

YoutubeJob::YoutubeJob(const KUrl& url, QByteArray& authKey, QMap<QString, QString>& videoInfo,QObject* parent)
	: KamosoJob(parent), url(url),m_authToken(authKey)
{
// 	m_authToken = authKey;
	setVideoInfo(videoInfo);
	developerKey = QByteArray(YOUTUBE_DEVELOPER_KEY);
}

void YoutubeJob::start()
{
	qDebug() << "File To Upload: " << url.path();
	openFileJob = KIO::get(url,KIO::NoReload,KIO::HideProgressInfo);
	connect(openFileJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(fileOpened(KIO::Job *, const QByteArray &)));
	openFileJob->start();
}

void YoutubeJob::fileOpened(KIO::Job *job, const QByteArray &data)
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
    finalData.append("<media:title type=\"plain\">"+m_videoInfo["videoTitle"]+"</media:title>\r\n");
    finalData.append("<media:description type=\"plain\">\r\n");
      finalData.append(m_videoInfo["videoDesc"]+"\r\n");
    finalData.append("</media:description>\r\n");
	finalData.append("<media:category\r\n");
      finalData.append("scheme=\"http://gdata.youtube.com/schemas/2007/categories.cat\">People\r\n");
    finalData.append("</media:category>\r\n");
    finalData.append("<media:keywords>"+m_videoInfo["videoTags"]+"</media:keywords>\r\n");
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

void YoutubeJob::moreData(KIO::Job *job, const QByteArray &data)
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
	job->suspend();
	qDebug() << "Upload Response" << data.data();
	QString dataStr(data);
	QRegExp rx("<media:player url='(\\S+)'/>");
	dataStr.contains(rx);
	qDebug() << rx.cap(1);
	url.setUrl(rx.cap(1));
	qDebug() << url.url();
	KToolInvocation::invokeBrowser(url.url());
	emit emitResult();
}

void YoutubeJob::setVideoInfo(QMap<QString, QString>& videoInfo)
{
	//This method will parse the content in the near future
	if(videoInfo["videoTitle"].size() > 0){
		
	}else{
		videoInfo["videoTitle"] = i18n("Video recorded using Kamoso");
	}
	if(videoInfo["videoDesc"].size() > 0){
		
	}else{
		videoInfo["videoDesc"] = i18n("This video has been recorded using Kamoso, a KDE software to play with webcams!");
	}
	if(videoInfo["videoTags"].size() > 0){
		
	}else{
		videoInfo["videoTags"] = "KDE, Kamoso";
	}
		
	m_videoInfo = videoInfo;
}

KIcon YoutubeJob::icon() const
{
	return KIcon("youtube");
}

QList<KUrl> YoutubeJob::urls() const
{
	return QList<KUrl>() << url;
}