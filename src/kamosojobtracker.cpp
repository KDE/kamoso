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

#include "kamosojobtracker.h"
#include "kamosojob.h"
#include <QPainter>
#include <KIcon>
#include <KUrl>
#include <KNotification>
#include <KLocale>

KamosoJobTracker::KamosoJobTracker(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{}

void KamosoJobTracker::registerJob(KamosoJob* job)
{
	connect(job, SIGNAL(result(KJob*)), SLOT(unregisterJob(KJob*)));
	mJobs.append(job);
	job->start();
	updateGeometry();
}

void KamosoJobTracker::unregisterJob(KJob* kjob)
{
	KamosoJob* job=static_cast<KamosoJob*>(kjob);
	mJobs.removeAll(job);
	updateGeometry();
	
	if(job->error()==0) {
		QStringList urls;
		foreach(const KUrl& url, job->urls()) {
			urls += url.prettyUrl();
		}
		KNotification::event(KNotification::Notification, i18n("Done: %1", urls.join(i18n(", "))),
							 job->icon().pixmap(48,48));
	} else
		KNotification::event(KNotification::Error, job->errorString());
}

static const int iconSide=16, separation=4;

QSize KamosoJobTracker::sizeHint() const
{
	return QSize((iconSide+separation)*mJobs.size(), iconSide);
}

void KamosoJobTracker::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	
	int i=0;
	foreach(KamosoJob* job, mJobs) {
		QRect target((iconSide+separation)*i++, 0, iconSide, iconSide);
		p.drawPixmap(target, job->icon().pixmap(target.size()));
	}
}
