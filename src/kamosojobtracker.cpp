/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#include <QMouseEvent>
#include <QPainter>

#include <KDebug>
#include <KIcon>
#include <KUrl>
#include <KJob>
#include <KNotification>
#include <KLocale>

KamosoJobTracker::KamosoJobTracker(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f), m_selectedJob(-1)
{
    setMouseTracking(true);
}

void KamosoJobTracker::registerJob(KJob* job, const KUrl::List& urls, const QIcon& icon)
{
    Q_ASSERT(!mJobs.contains(job));
    kDebug() << "Register job received!!!";
    connect(job, SIGNAL(result(KJob*)), SLOT(unregisterJob(KJob*)));
    connect(job, SIGNAL(percent(KJob*,ulong)), SLOT(repaint()));
    mJobs.insert(job, qMakePair(urls, icon));
    job->start();
    updateGeometry();
    emit urlsChanged(urls);
}

void KamosoJobTracker::unregisterJob(KJob* job)
{
    QPair< KUrl::List, QIcon > val = mJobs.take(job);
    updateGeometry();

    if(job->error()==0) {
        QStringList urls;
        foreach(const KUrl& url, val.first) {
            urls += url.prettyUrl();
        }
        KNotification::event(KNotification::Notification, i18n("Done: %1", urls.join(i18nc("Used to join urls", ", "))),
                                val.second.pixmap(48,48));
    } else {
        KNotification::event(KNotification::Error, job->errorString());
    }

    emit urlsChanged(val.first);
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
    QPixmap alphamask(iconSide, iconSide);
    alphamask.fill(Qt::gray);
    foreach(KJob* job, mJobs.keys()) {
        QRect target((iconSide+separation)*i, 0, iconSide, iconSide);
        if(i==m_selectedJob) //Make it nicer
            p.drawRect(target);

        int completedPix=(job->percent()*iconSide)/100;
        QRect source(0,0, iconSide, completedPix);
        QRect sourceRest(0,completedPix, iconSide, iconSide-completedPix);
        QRect comptarget((iconSide+separation)*i, 0, iconSide, completedPix);
        QRect resttarget((iconSide+separation)*i, completedPix, iconSide, iconSide-completedPix);

        QPixmap icon=mJobs.value(job).second.pixmap(target.size());
        p.drawPixmap(comptarget, icon, source);

        icon.setAlphaChannel(alphamask);
        p.drawPixmap(resttarget, icon, sourceRest);
        i++;
    }
}

void KamosoJobTracker::mousePressEvent(QMouseEvent* ev)
{
    int i=jobPerPosition(ev->pos());
    if(i>=0 && i<mJobs.size()) {
        KJob* job = mJobs.keys()[i];
        emit jobClicked(job, mJobs.value(job).first);
    }
}

int KamosoJobTracker::jobPerPosition(const QPoint& pos)
{
    int x=pos.x();
    return x/(iconSide+separation);
}

void KamosoJobTracker::mouseMoveEvent(QMouseEvent* ev)
{
    setSelectedJob(jobPerPosition(ev->pos()));
}

void KamosoJobTracker::leaveEvent(QEvent*)
{
    setSelectedJob(-1);
}

void KamosoJobTracker::setSelectedJob(int newselection)
{
    if(newselection!=m_selectedJob) {
        m_selectedJob=newselection;
        repaint();
    }
}

QList<QIcon> KamosoJobTracker::iconsPerUrl(const KUrl& url) const
{
    QList<QIcon> ret;
    typedef QPair<KUrl::List, QIcon> pair;
    foreach(const pair& ue, mJobs) {
        if(ue.first.contains(url))
            ret += ue.second;
    }
    return ret;
}
