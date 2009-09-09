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

#include "execute.h"
#include <KPluginFactory>
#include <KAboutData>
#include <KIcon>
#include <KUrl>
#include <QAction>
#include <QDesktopServices>
#include <KMessageBox>

K_PLUGIN_FACTORY(KamosoExecuteFactory, registerPlugin<ExecutePlugin>(); )
K_EXPORT_PLUGIN(KamosoExecuteFactory(KAboutData("execute", "execute",
		ki18n("Execute"), "0.1", ki18n("Runs a viewer for the specified file."),
		KAboutData::License_GPL)))

ExecutePlugin::ExecutePlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args)
{}

QAction* ExecutePlugin::thumbnailsAction(const QList<KUrl>& urls)
{
	QAction* act=0;
	mSelectedUrls.clear();
	foreach(const KUrl& url, urls)
	{
		if(!act) {
			act=new QAction(KIcon("system-run"), i18n("Open..."), 0);
			connect(act, SIGNAL(triggered(bool)), SLOT(execute(bool)));
		}
		
		mSelectedUrls.append(url);
	}
	return act;
}

void ExecutePlugin::execute(bool)
{
	foreach(const KUrl& path, mSelectedUrls) {
		bool corr=QDesktopServices::openUrl(path);
		
		if(!corr)
			KMessageBox::error(0, i18n("Could not open %1", path.prettyUrl()));
	}
}
