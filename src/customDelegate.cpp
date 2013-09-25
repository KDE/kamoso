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

#include "customDelegate.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QDebug>
#include <KDirModel>

KIcon CustomDelegate::m_unavailable;
QColor shadowColor(100, 100, 100);

CustomDelegate::CustomDelegate(const QHash< KUrl, QPixmap >& repo, QWidget* parent)
    : QItemDelegate(parent), m_repo(repo)
{
    if(m_unavailable.isNull()) {
        m_unavailable=KIcon("image-missing");
    }
}

CustomDelegate::~CustomDelegate()
{}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QRect& rect = option.rect;

    QStyleOptionViewItemV4 opt(option);
    QStyle *style = opt.widget->style();

    KFileItem file = qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole));
    KUrl url = file.url();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
    QList<QIcon> icons = m_overlays.value(url);

    const int overlayperline = 5;
    int space = qMin(rect.width(), rect.height());
    int extent = space/overlayperline;

    if(m_repo.contains(url))
    {
        QSize s= m_repo[url].size();
        if(!icons.isEmpty())
            s.scale(s.width(), s.height()-extent, Qt::KeepAspectRatio);
        QPoint topleft(rect.topLeft().x()+(rect.width()-s.width())/2,
                        rect.topLeft().y()+(rect.height()-s.height())/2);
        if(!icons.isEmpty())
            topleft.ry() -= extent;

        QPainterPath path(topleft);
        path.lineTo(QPoint(s.width()   + rect.left(), topleft.y()));
        path.quadTo(QPoint(s.width()/4 + rect.left(), s.height()/4 + topleft.y()),
                    QPoint(topleft.x(), s.height() + topleft.y()));
        path.lineTo(topleft);

        QRect pixRect(topleft, s);
        pixRect.adjust(3,3, -7,-7);
        QRect shadowRect=pixRect;
        shadowRect.translate(3,3);
        painter->setBrush(shadowColor);
        painter->drawRect(shadowRect);
        painter->drawPixmap(pixRect, m_repo[url]);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillPath(path, QColor(255, 255, 255, 25));
    }
    else
    {
        emit pixmapNeeded(file, index, rect);

        QPixmap pix = m_unavailable.pixmap(qMin(rect.width(), rect.height()));
        QSize s=pix.size();
        QPoint topleft(rect.topLeft().x()+(rect.width()-s.width())/2,
                        rect.topLeft().y()+(rect.height()-s.height())/2);
        painter->drawPixmap(QRect(topleft, s), pix);
    }

    if(option.showDecorationSelected)
    {
        painter->setBrush(Qt::red);
        painter->drawRect(rect);
    }

    if(!icons.isEmpty()) {
        int count=0;

        foreach(const QIcon& icon, icons) {
            QPixmap pix = icon.pixmap(extent);
            QPoint tl = rect.bottomLeft() - QPoint(0,extent);
            tl.rx() += (count%overlayperline) * extent;
    // 			tl.ry() += (count/overlayperline)*pix.height();
            if(count/overlayperline>=1)
                break;

            painter->drawPixmap(tl, pix);

            count++;
        }
    }
}

QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option); Q_UNUSED(index);
    return QSize(100, 125);
}

void CustomDelegate::setOverlays(const KUrl& url, const QList< QIcon >& icons)
{
    m_overlays.insert(url, icons);
}
