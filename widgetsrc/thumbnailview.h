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

#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QListView>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QScrollBar>
#include <QCursor>

#include <kio/previewjob.h>
#include "customDelegate.h"
#include <QPropertyAnimation>

class KamosoTracker;

class ThumbnailView : public QListView
{
Q_OBJECT
    public:
        ThumbnailView(QWidget* parent = 0);

        virtual QStyleOptionViewItem viewOptions () const
        {
            QStyleOptionViewItem options=QListView::viewOptions();
            options.decorationPosition = QStyleOptionViewItem::Top;
            return options;
        }

        void assignDelegate();
        QSize sizeHint() const { return QSize(150,150); }
        void setXValue(int v);
        int xValue() const { return m_xProperty->endValue().toInt(); }

        CustomDelegate* delegate() const;
        void clearWaiting();

    protected:
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent (QMouseEvent* event );

    private slots:
        void previewAvailable(const KFileItem& file, const QPixmap& pic);
        void retrievePixmap(const KFileItem& it, const QModelIndex & idx, const QRect& rect);
        void updatexClick(const QModelIndex & idx);

    private:
        QHash<KUrl, QPixmap> m_repo;
        QMap<KUrl, QModelIndex> m_waiting;
        QPropertyAnimation* m_xProperty;
        int xClick;
};

#endif
