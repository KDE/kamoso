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

#include "youtube.h"
#include <KPluginFactory>
#include <KAboutData>
#include <KMimeType>
#include <KIcon>
#include <KUrl>
#include <KMessageBox>

#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include "youtubemanager.h"

K_PLUGIN_FACTORY(KamosoYoutubeFactory, registerPlugin<YoutubePlugin>(); )
K_EXPORT_PLUGIN(KamosoYoutubeFactory(KAboutData("youtube", "youtube",
		ki18n("Youtube"), "0.1", ki18n("Runs a visor for the specified file."),
		KAboutData::License_GPL)))

YoutubePlugin::YoutubePlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args)
{}

QAction* YoutubePlugin::thumbnailsAction(const QList<KUrl>& urls)
{
	QAction* act=0;
	mSelectedUrls.clear();
	foreach(const KUrl& url, urls)
	{
		KMimeType::Ptr mime = KMimeType::findByUrl(url);
		if(mime->name().startsWith("video/")) {
			if(!act) {
				act=new QAction(KIcon("system-run"), i18n("Upload to youtube"), 0);
				connect(act, SIGNAL(triggered(bool)), SLOT(upload(bool)));
			}
		}
		mSelectedUrls.append(url);
	}
	return act;
}

void YoutubePlugin::upload(bool)
{
	QByteArray username("tetasnor");
	QByteArray password("12344321");
	QByteArray developerKey("AI39si41ZFrIJoZGNH0hrZPhMuUlwHc6boMLi4e-_W6elIzVUIeDO9F7ix2swtnGAiKT4yc4F4gQw6yysTGvCn1lPNyli913Xg");
	m_manager = new YoutubeManager(username,password,developerKey);
	connect(m_manager,SIGNAL(authenticated(bool)),this,SLOT(authenticated(bool)));
	m_manager->login();
// 	authenticated(true);
}

void YoutubePlugin::authenticated(bool auth)
{
	qDebug() << "Authentification: " << auth ;
	if(auth == false){
		//TODO to be done
		return;
	}
	foreach(const KUrl& path, mSelectedUrls) {
		m_manager->upload(path);
	}
	connect(m_manager,SIGNAL(uploadDone(bool)),this,SLOT(uploadDone(bool)));
}

void YoutubePlugin::uploadDone(bool auth)
{
	qDebug() << "UploadDone: " << auth;
}