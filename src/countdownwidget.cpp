/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "countdownwidget.h"
#include <QShowEvent>

#include <KWindowSystem>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <qtimer.h>

CountdownWidget::CountdownWidget(QWidget* parent) : QWidget(parent)
{
	connect(this, SIGNAL(pressed()), SLOT(hide()));
	
	currentState=Red;
}

void CountdownWidget::start()
{
// 	if(!KWindowSystem::compositingActive())
// 	{
// 		background=QPixmap::grabWindow(QApplication::desktop()->winId());
// 	}
// 	else
// 		background.fill(Qt::transparent);
	currentState=Red;
	
	QTimer::singleShot(1*500, this, SLOT(currentYellow()));
	QTimer::singleShot(2*500, this, SLOT(currentGreen()));
	QTimer::singleShot(3*500, this, SLOT(hide()));
	QTimer::singleShot(3*500, this, SIGNAL(finished()));
}

void CountdownWidget::currentYellow()
{
	currentState=Yellow;
	repaint();
}

void CountdownWidget::currentGreen()
{
	currentState=Green;
	repaint();
}

void CountdownWidget::paintEvent (QPaintEvent* )
{
	QPainter painter(this);
	
// 	painter.drawPixmap(ev->rect(), background);
	
	int rad=qMin(height()/2, 100);
	int dist=width()/3;
	
	switch(currentState) {
		case Green:
			painter.setPen(Qt::green);
			painter.setBrush(Qt::green);
			painter.drawEllipse(rect().center()+QPoint(dist, 0), rad, rad);
		case Yellow:
			painter.setPen(Qt::yellow);
			painter.setBrush(Qt::yellow);
			painter.drawEllipse(rect().center(), rad, rad);
		case Red:
			painter.setPen(Qt::red);
			painter.setBrush(Qt::red);
			painter.drawEllipse(rect().center()-QPoint(dist, 0), rad, rad);
	}
}
