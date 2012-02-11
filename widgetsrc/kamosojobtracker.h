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

#ifndef KAMOSOJOBTRACKER_H
#define KAMOSOJOBTRACKER_H
#include <QWidget>
#include <KUrl>

class KJob;

class KamosoJobTracker
    : public QWidget
{
Q_OBJECT
    public:
        KamosoJobTracker(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual void mousePressEvent(QMouseEvent* ev);
        virtual void mouseMoveEvent(QMouseEvent* );
        virtual void leaveEvent(QEvent* );

        QList<QIcon> iconsPerUrl(const KUrl& url) const;

    public slots:
        void registerJob(KJob* job, const KUrl::List& urls, const QIcon& icon);
        void unregisterJob(KJob* job);

        virtual QSize sizeHint() const;
        virtual void paintEvent(QPaintEvent* );
    signals:
        void urlsChanged(const KUrl::List& urls);
        void jobClicked(KJob* job, const KUrl::List& urls);

    private:
        void setSelectedJob(int newselection);
        int jobPerPosition(const QPoint& pos);
        QMap<KJob*, QPair<KUrl::List, QIcon> > mJobs;
        int m_selectedJob;
};

#endif
