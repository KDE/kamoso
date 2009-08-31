/*************************************************************************************
 *  Copyright (C) 2009 by Aleix Pol <aleixpol@kde.org>                               *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 3                   *
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

#include "facebook.h"
#include <KPluginFactory>
#include <KAboutData>
#include <KMimeType>
#include <KDebug>
#include <QAction>

//Copied from the facebook kipiplugin
// #include "fbtalker.h"
#include "fbalbum.h"

K_PLUGIN_FACTORY(KDevExecuteFactory, registerPlugin<FacebookPlugin>(); )
K_EXPORT_PLUGIN(KDevExecuteFactory(KAboutData("facebooksender", "facebooksender",
		ki18n("Facebook support"), "0.1", ki18n("Allows to communicate with Facebook"),
		KAboutData::License_GPL)))

using namespace KIPIFacebookPlugin;

FacebookPlugin::FacebookPlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args)
{}

QAction* FacebookPlugin::thumbnailsAction(const KUrl& url)
{
	KMimeType::Ptr mime = KMimeType::findByUrl(url);
	QAction* act=0;
	mSelectedUrls.clear();
	if(mime->name().startsWith("image/")) {
		act=new QAction(i18n("Upload to Facebook..."), 0);
		connect(act, SIGNAL(triggered(bool)), SLOT(uploadImage(bool)));
		
		mSelectedUrls=url;
	}
	return act;
}

void FacebookPlugin::uploadImage(bool)
{
	Q_ASSERT(!mSelectedUrls.isEmpty());
	kDebug() << "uploading..." << mSelectedUrls;
	
	FacebookJob* job=new FacebookJob(mSelectedUrls);
	emit jobCreated(job);
	job->exec();
}

FacebookJob::FacebookJob(const KUrl& url, QObject* parent)
	: KJob(parent), url(url), talk(0)
{
	Q_ASSERT(url.isLocalFile()); //TODO: Move to temp otherwise
	
	connect(&talk, SIGNAL(signalLoginDone(int,QString)),
			this, SLOT(loginDone(int,QString)));
	connect(&talk, SIGNAL(signalListAlbumsDone(int,QString,QList<KIPIFacebookPlugin::FbAlbum>)),
			this, SLOT(albumList(int,QString,QList<KIPIFacebookPlugin::FbAlbum>)));
	connect(&talk, SIGNAL(signalCreateAlbumDone(int,QString,long long)),
			this, SLOT(albumCreated(int,QString,long long)));
}

void FacebookJob::start()
{
	KConfig cfg(KGlobal::mainComponent());
	KConfigGroup cfgGroup=cfg.group("Facebook");
	QString sessionKey=cfgGroup.readEntry("Key", QString());
	QString sessionSecret=cfgGroup.readEntry("Secret", QString());
	uint sessionExpires=cfgGroup.readEntry("Expires", 0);
	
	talk.authenticate(sessionKey, sessionSecret, sessionExpires);
}

void FacebookJob::loginDone(int, const QString& )
{
	KConfig cfg(KGlobal::mainComponent());
	KConfigGroup cfgGroup=cfg.group("Facebook");
	cfgGroup.writeEntry("Key", talk.getSessionKey());
	cfgGroup.writeEntry("Secret", talk.getSessionSecret());
	cfgGroup.writeEntry("Expires", talk.getSessionExpires());
	cfgGroup.sync();
	
	qDebug() << "logged in" << talk.getSessionExpires();
	talk.listAlbums();
}

void FacebookJob::albumList(int errCode, const QString& errMsg, const QList<FbAlbum>& albums)
{
	long long id=-1;
	foreach(const FbAlbum& album, albums) {
		if(album.title==i18n("Kamoso")) {
			id=album.id;
			break;
		}
	}
	
	if(id==-1) {
		FbAlbum album;
		album.title=i18n("Kamoso");
		album.description=i18n("Photos taken with the webcam");
		
		talk.createAlbum(album);
	} else
		sendPhoto(id);
	qDebug() << "listed" << id;
}


void FacebookJob::albumCreated(int errCode, const QString& error, long long albumId)
{
	sendPhoto(albumId);
	qDebug() << "album created" << albumId;
}

void FacebookJob::sendPhoto(long long album)
{
	bool c=talk.addPhoto(url.toLocalFile(), album, url.fileName());
	Q_ASSERT(c && "could not add the photo to the album");
	
	emit emitResult();
}
