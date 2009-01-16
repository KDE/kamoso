/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@gmail.com>                             *
 *  Copyright (C) 2008 by Alex Fiestas <alex@eyeos.org>                              *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 3                   *
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

#include "customDelegate.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QDebug>
#include <KDirModel>

KIcon CustomDelegate::m_unavailable;

CustomDelegate::CustomDelegate(const QHash<KUrl, QPixmap>& repo, QWidget *parent)
  : QItemDelegate(parent), m_repo(repo)
{
	if(m_unavailable.isNull())
		m_unavailable=KIcon("user-away");
}

CustomDelegate::~CustomDelegate()
{}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const QRect& rect = option.rect;
	painter->setRenderHint(QPainter::Antialiasing);
	
	KFileItem file = qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole));
	KUrl url = file.url();
	
	if(m_repo.contains(url))
	{
		QPainterPath path(QPoint(rect.left(), rect.top()));
		path.lineTo(QPoint(rect.width() + rect.left(), rect.top()));
		path.quadTo(QPoint(rect.width() / 4 + rect.left(), rect.height() / 4 + rect.top()),
					QPoint(rect.left(), option.rect.height() + rect.top()));
		path.lineTo(QPoint(rect.left(), rect.top()));
		
		painter->drawPixmap(rect, m_repo[url]);
		painter->fillPath(path, QColor(255, 255, 255, 75));
	}
	else
	{
		emit pixmapNeeded(file, index, option.rect);
		
// 		painter->setBrush(Qt::blue);
// 		painter->drawRect(rect);
		
		painter->drawPixmap(rect, m_unavailable.pixmap(rect.size()));
	}
	
	if(option.showDecorationSelected)
	{
		
		painter->setBrush(Qt::red);
		painter->drawRect(rect);
	}
}

QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	return QSize(125,125);
}

