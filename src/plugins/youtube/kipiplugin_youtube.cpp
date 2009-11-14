/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
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

#include <KPluginFactory>
#include <KAboutData>
#include <KMimeType>
#include <KIcon>
#include <KUrl>
#include <KDialog>
#include <KMessageBox>
#include <kpassworddialog.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <kwallet.h>
#include "kipiplugin_youtube.h"
#include "youtubejob.h"
#include <libkipi/interface.h>

using KWallet::Wallet;

K_PLUGIN_FACTORY(KamosoYoutubeFactory, registerPlugin<YoutubePlugin>(); )
K_EXPORT_PLUGIN(KamosoYoutubeFactory(KAboutData("kipiplugin_youtube", "kipiplugin_youtube",
		ki18n("YouTube"), "0.1", ki18n("Uploads files to YouTube"),
		KAboutData::License_GPL)))

YoutubePlugin::YoutubePlugin(QObject* parent, const QVariantList& args)
	: KIPI::Plugin(KamosoYoutubeFactory::componentData(),parent, "Youtube")
{
	KIconLoader::global()->addAppDir("kamoso_youtube");
}

// QAction* YoutubePlugin::thumbnailsAction(const QList<KUrl>& urls)
// {
// 	QAction* act=0;
// 	mSelectedUrls.clear();
// 	foreach(const KUrl& url, urls)
// 	{
// 		KMimeType::Ptr mime = KMimeType::findByUrl(url);
// 		if(mime->name().startsWith("video/")) {
// 			if(!act) {
// 				act=new QAction(KIcon("youtube"), i18n("Upload to YouTube"), 0);
// 				connect(act, SIGNAL(triggered(bool)), SLOT(upload()));
// 			}
// 		}
// 		mSelectedUrls.append(url);
// 	}
// 	return act;
// }

KJob* YoutubePlugin::exportFiles(const QString& albumname)
{
	KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
	return new YoutubeJob(interface->currentSelection().images());
}

KIPI::Category YoutubePlugin::category(KAction* action) const
{
	return KIPI::ExportPlugin;
}

void YoutubePlugin::setup(QWidget* widget)
{
	KIPI::Plugin::setup(widget);
}