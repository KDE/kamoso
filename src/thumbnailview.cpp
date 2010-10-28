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

#include "thumbnailview.h"

ThumbnailView::ThumbnailView(QWidget* parent) : QListView(parent)
{
	connect(this, SIGNAL(pressed (QModelIndex)), SLOT(updatexClick(QModelIndex)));
}

void ThumbnailView::previewAvailable(const KFileItem& file, const QPixmap& pic)
{
	m_repo.insert(file.url(), pic);
	update(m_waiting.take(file.url()));
}

void ThumbnailView::assignDelegate()
{
	CustomDelegate* c=new CustomDelegate(m_repo, this);
	connect(c, SIGNAL(pixmapNeeded(KFileItem,QModelIndex,QRect)), SLOT(retrievePixmap(KFileItem,QModelIndex,QRect)));
	setItemDelegate(c);
}

void ThumbnailView::retrievePixmap(const KFileItem& file, const QModelIndex& idx, const QRect& rect)
{
	m_waiting.insert(file.url(), idx);
	
	KIO::PreviewJob* job = new KIO::PreviewJob(KFileItemList() << file, rect.width(), rect.height(), 0, 0, true, false, 0);
	connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(previewAvailable(KFileItem,QPixmap)));
	job->setAutoDelete(true);
	job->start();
}

void ThumbnailView::updatexClick(const QModelIndex & idx)
{
	Q_UNUSED(idx);
	xClick= QCursor::pos().x(); 
}

void ThumbnailView::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		int x= QCursor::pos().x(); 
		int difx = (x - xClick);
		int v = this->horizontalScrollBar()->value();
		int min=this->horizontalScrollBar()->minimum();
		int max=this->horizontalScrollBar()->maximum();
		xClick= x;
		setCursor(Qt::SizeHorCursor);
		horizontalScrollBar()->setValue(qBound(min, v-difx, max));
	}
}

void ThumbnailView::mouseReleaseEvent ( QMouseEvent * event ) 
{
	Q_UNUSED(event);
	this->setCursor(QCursor(Qt::ArrowCursor));
}
