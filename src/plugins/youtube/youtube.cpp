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
#include <KIcon>
#include <KUrl>
#include <QAction>
#include <QDesktopServices>
#include <KMessageBox>
#include <KMimeType>
#include <QDebug>
#include <glib.h>
#include <glib-object.h>

extern "C" {
	#include <gdata/services/youtube/gdata-youtube-service.h>
	#include <gcal.h>
#include <gcalendar.h>
}

K_PLUGIN_FACTORY(KamosoYoutubeFactory, registerPlugin<YoutubePlugin>(); )
K_EXPORT_PLUGIN(KamosoYoutubeFactory(KAboutData("Youtube", "youtube",
		ki18n("Youtube"), "0.1", ki18n("Upload videos directly to youtube"),
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
	g_type_init();
	GDataYouTubeService* service;
	service == NULL;
	const char* lol = "AI39si41ZFrIJoZGNH0hrZPhMuUlwHc6boMLi4e-_W6elIzVUIeDO9F7ix2swtnGAiKT4yc4F4gQw6yysTGvCn1lPNyli913Xg";
	const char* bar = "ytapi-GNOME-Totem-444fubtt";
	service = gdata_youtube_service_new(lol,bar);
	if(service == NULL){
		qDebug() << "KAKA PA TOS";
	}else{
		qDebug() << "ALALAL";
	}
	if(gdata_service_authenticate(GDATA_SERVICE(service),"tetasnor","12344321",NULL,NULL))
	{
		qDebug() << "Autenticated!!!!\n\n\n\n\n\n\n\n";
 		GDataYouTubeVideo *video = gdata_youtube_video_new(NULL);
		GFile *file = g_file_new_for_path("/home/nasete/Pictures/Webcam/kamoso_03212009_172157.ogv");
		gdata_youtube_service_upload_video(service,video,file,NULL,NULL);
	}else{
		qDebug() << "Ressadasdasdaduuuult: \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
}
