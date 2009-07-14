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

#include "whitewidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <KLocale>
#include <KWindowSystem>

const int m_steps(10), m_ini(15);
WhiteWidget::WhiteWidget(QWidget* parent)
	: QWidget(parent)
{
	setAutoFillBackground(false);
	m_timer = new QTimer(this);
	
	connect(m_timer, SIGNAL(timeout()), SLOT(tick()));
}


void WhiteWidget::paintEvent (QPaintEvent* paintEvent)
{
	QPainter p(this);
	
	p.setBrush(Qt::white);
	p.drawRect(paintEvent->rect());
	p.drawText(paintEvent->rect().center(), i18n("Smile! :)"));
}

void WhiteWidget::showEvent(QShowEvent * event)
{
	m_currentStep=0;
	m_timer->start(30);
}

void WhiteWidget::closeEvent(QCloseEvent* event)
{
	m_timer->stop();
}

void WhiteWidget::tick()
{
	m_currentStep=qMin(m_currentStep+1, m_steps);
	
	if(KWindowSystem::compositingActive())
		setWindowOpacity(1/m_currentStep);
}
