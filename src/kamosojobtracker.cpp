/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
	
	for(int i=0; i<mJobs.size(); ++i) {
		QRect target((iconSide+separation)*i, 0, iconSide, iconSide);
		p.drawPixmap(target, mJobs[i]->icon().pixmap(target.size()));
	}
}
