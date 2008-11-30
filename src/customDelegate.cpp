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
#include <QPixmap>
#include <QDebug>
#include <KDirModel>

CustomDelegate::CustomDelegate(QWidget* parent) : QItemDelegate(parent)
{

}
CustomDelegate::~CustomDelegate(){

}
void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
	KFileItem file = qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole)) ;
	QString url = file.url().pathOrUrl();
	QImage image(url);
	QRect rect = option.rect;
	rect.setX(rect.x() +10);
	rect.setY(rect.y() +20);
	rect.setWidth(105);
	rect.setHeight(105);
	painter->setBrush(QBrush(QColor("#D9D7D6")));
	painter->drawRect(option.rect);
 	painter->drawImage(rect,image);

}
QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const
{
	return QSize(125,125);
}