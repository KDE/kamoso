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

#include "thumbnailview.h"

ThumbnailView::ThumbnailView(QWidget* parent) : QListView(parent)
{
    setHorizontalScrollMode(ScrollPerPixel);
    m_xProperty=new QPropertyAnimation(horizontalScrollBar(), "value", this);

    connect(this, SIGNAL(pressed(QModelIndex)), SLOT(updatexClick(QModelIndex)));
}

void ThumbnailView::previewAvailable(const KFileItem& file, const QPixmap& pic)
{
    if(m_waiting.contains(file.url())) {
        m_repo.insert(file.url(), pic);
        update(m_waiting.take(file.url()));
    }
}

void ThumbnailView::assignDelegate()
{
    CustomDelegate* c=new CustomDelegate(m_repo, this);
    connect(c, SIGNAL(pixmapNeeded(KFileItem,QModelIndex,QRect)), SLOT(retrievePixmap(KFileItem,QModelIndex,QRect)));
    setItemDelegate(c);
}

void ThumbnailView::retrievePixmap(const KFileItem& file, const QModelIndex& idx, const QRect& rect)
{
    if(!m_waiting.contains(file.url())) {
        m_waiting.insert(file.url(), idx);

        KIO::PreviewJob* job = new KIO::PreviewJob(KFileItemList() << file, rect.width(), rect.height(), 0, 0, true, false, 0);
        connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(previewAvailable(KFileItem,QPixmap)));
        job->setAutoDelete(true);
        job->start();
    }
}

void ThumbnailView::updatexClick(const QModelIndex & idx)
{
    Q_UNUSED(idx);
    xClick= QCursor::pos().x(); 
}

void ThumbnailView::mousePressEvent(QMouseEvent* event)
{
    m_xProperty->setEndValue(horizontalScrollBar()->value());
    QAbstractItemView::mousePressEvent(event);
}

void ThumbnailView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int x= QCursor::pos().x();
        int difx = 3*(x - xClick);
        int v = m_xProperty->endValue().toInt();
        xClick= x;
        setCursor(Qt::SizeHorCursor);
        setXValue(v-difx);
    }
}

void ThumbnailView::setXValue(int v)
{
    int min=horizontalScrollBar()->minimum();
    int max=horizontalScrollBar()->maximum();
    int value = qBound(min, v, max);

    m_xProperty->setEndValue(value);
    m_xProperty->stop();
    m_xProperty->start();
}

void ThumbnailView::mouseReleaseEvent ( QMouseEvent * event ) 
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::ArrowCursor));
}

CustomDelegate* ThumbnailView::delegate() const
{
    return qobject_cast<CustomDelegate*>(itemDelegate());
}

void ThumbnailView::clearWaiting()
{
    m_waiting.clear();
}
