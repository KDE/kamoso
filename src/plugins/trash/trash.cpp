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

#include "trash.h"
#include <KPluginFactory>
#include <KAboutData>
#include <KIcon>
#include <KUrl>
#include <KMessageBox>
#include <kio/job.h>
#include <kio/copyjob.h>

#include <QAction>
#include <QDesktopServices>
#include <kjobuidelegate.h>

K_PLUGIN_FACTORY(KamosoTrashFactory, registerPlugin<TrashPlugin>(); )
K_EXPORT_PLUGIN(KamosoTrashFactory(KAboutData("trash", "trash",
		ki18n("Trash"), "0.1", ki18n("Moves the specified file to trash."),
		KAboutData::License_GPL)))

TrashPlugin::TrashPlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args)
{}

QAction* TrashPlugin::thumbnailsAction(const QList<KUrl>& urls)
{
	QAction* act=0;
	mSelectedUrls.clear();
	foreach(const KUrl& url, urls)
	{
		if(!act) {
			act=new QAction(KIcon("trash-empty"), i18n("Move to trash..."), 0);
			connect(act, SIGNAL(triggered(bool)), SLOT(trash(bool)));
		}
		
		mSelectedUrls.append(url);
	}
	return act;
}

void TrashPlugin::trash(bool)
{
	int res=KMessageBox::warningContinueCancel(0,
						i18n("Are you sure you want to delete these files?"),
						i18n("Move to Trash"));
	
	if(res==KMessageBox::Continue) {
		KIO::CopyJob *job = KIO::trash(mSelectedUrls);
		connect( job, SIGNAL(result(KJob *)), this, SLOT(slotResult(KJob *)));
		
		//TODO: Eventually move to KamosoJob
		job->start();
	}
}

void TrashPlugin::slotResult(KJob* job)
{
	if ( job->error() )
		job->uiDelegate()->showErrorMessage();
}
