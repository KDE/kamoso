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

#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QItemDelegate>
#include <QHash>
#include <KUrl>
#include <KIcon>

class KJob;
class KFileItem;

class CustomDelegate : public QItemDelegate
{
Q_OBJECT
    public:
        CustomDelegate(const QHash<KUrl, QPixmap>& repo, QWidget *parent = 0);
        ~CustomDelegate();
        void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;
        void setOverlays(const KUrl& url, const QList< QIcon >& icons);

    Q_SIGNALS:
        void pixmapNeeded(const KFileItem& it, const QModelIndex& idx, const QRect&) const;

    private:
        const QHash<KUrl, QPixmap>& m_repo;
        QHash<KUrl, QList<QIcon> > m_overlays;
        static KIcon m_unavailable;
};

#endif
