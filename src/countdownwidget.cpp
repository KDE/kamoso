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

#include "countdownwidget.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QTimeLine>

static const int numberOfColours=3;
static const QColor colors[numberOfColours]={ Qt::red, Qt::yellow, Qt::green };

CountdownWidget::CountdownWidget(QWidget* parent)
    : QWidget(parent)
{
    mTimer=new QTimeLine(1, this);
    connect(mTimer, SIGNAL(valueChanged(qreal)), SLOT(tick(qreal)));
    connect(mTimer, SIGNAL(finished()), this, SLOT(hide()));
    connect(mTimer, SIGNAL(finished()), this, SIGNAL(finished()));

    mTimer->setCurveShape(QTimeLine::EaseInCurve);
}

void CountdownWidget::start(int timeInterval)
{
    mTimer->setDuration(timeInterval);
    mTimer->start();
}

void CountdownWidget::stop()
{
    mTimer->stop();
}

void CountdownWidget::tick(qreal progress)
{
    mProgress=progress;
    repaint();
}

void CountdownWidget::hideEvent(QHideEvent* )
{
    mTimer->stop();
    mTimer->setCurrentTime(0);
}

void CountdownWidget::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int margin=5;
    int rad=height()/2-margin;
    int dist=(width()-rad*numberOfColours)/(numberOfColours-1);

    int current=int(mProgress*numberOfColours);
    
    QFont f = font();
    f.setPointSize(20);
    setFont(f);
    
    for(int i=0; i<numberOfColours; i++) {
        QColor realColor = colors[i];
        QColor darkColor = realColor.dark(125);
        QColor color = i>=current ? darkColor : realColor;

        QPointF tl(margin+dist*i, margin);
        QPointF ellipseCenter=tl+QPointF(rad, height()/2-margin);
        QRectF ellipse(tl, QSizeF(rad*2, rad*2));

        painter.setPen(QPen(color.dark(), 3));
        painter.setBrush(color);
        painter.drawEllipse(ellipse);

        if(current==i) {
            painter.setPen(realColor);
            painter.setBrush(realColor);

            double progUnit=1./numberOfColours;
            double prog=(mProgress-i*progUnit)/progUnit;

            painter.drawEllipse(ellipseCenter, rad*prog, rad*prog);
        }
        
        painter.setPen(darkColor);
        painter.drawText(ellipse, QString::number(numberOfColours-i), QTextOption(Qt::AlignCenter));
    }
}
