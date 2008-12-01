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

CustomDelegate::CustomDelegate(QWidget* parent) : QItemDelegate(parent)
{

}

CustomDelegate::~CustomDelegate(){

}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  painter->setRenderHint(QPainter::Antialiasing);

  KFileItem file = qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole));
  const QString url = file.url().pathOrUrl();
  const QImage image(url);
  const QRect rect = option.rect;
  QPainterPath path(QPoint(rect.left(), rect.top()));
  path.lineTo(QPoint(rect.width() + rect.left(), rect.top()));
  path.quadTo(QPoint(rect.width() / 4 + rect.left(), rect.height() / 4 + rect.top()), QPoint(rect.left(), option.rect.height() + rect.top()));
  path.lineTo(QPoint(rect.left(), rect.top()));
  painter->drawImage(rect, image);
  painter->fillPath(path, QColor(255, 255, 255, 100));
}

QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const
{
  return QSize(125,125);
}
