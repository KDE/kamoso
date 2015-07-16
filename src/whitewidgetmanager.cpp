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

#include "whitewidgetmanager.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QGuiApplication>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>

/**
*This class create and manage 1 white widget per screen, creating an unified interface for all of them
*/
WhiteWidgetManager::WhiteWidgetManager(QObject* parent) : QObject(parent)
{
    createWhiteWidgets();

    m_timer = new QPropertyAnimation(this);
    m_timer->setDuration(500);
    m_timer->setStartValue(0.);
    m_timer->setEndValue(1.0);
    m_timer->setTargetObject(this);
    m_timer->setPropertyName("opacity");
    connect(m_timer, SIGNAL(finished()), SLOT(hideAll()));
}

/**
*This method detect the current screens and create one whiteWidget per screen
*/
void WhiteWidgetManager::createWhiteWidgets()
{
    QList<QScreen*> screens = qGuiApp->screens();

    Q_FOREACH (QScreen* screen, screens)
    {
        WhiteWidget *whiteWidget = new WhiteWidget;
        whiteWidget->setScreen(screen);
        whiteWidget->setGeometry(screen->geometry());
        whitewidgetList.append(whiteWidget);
    }
}

/**
*This method show all whiteWidgets, and start the unified Qtimer
*/
void WhiteWidgetManager::showAll()
{
    m_timer->start();

    Q_FOREACH(WhiteWidget *iteratorWidget, whitewidgetList)
    {
        iteratorWidget->setOpacity(0);
        iteratorWidget->showFullScreen();
    }
}

/**
*This method is usually called by timeout, and hide all the whiteWidgets 
*/
void WhiteWidgetManager::hideAll()
{
    Q_FOREACH(WhiteWidget *iteratorWidget, whitewidgetList)
    {
        iteratorWidget->hide();
    }
    m_timer->stop();
}

void WhiteWidgetManager::setOpacity(qreal op)
{
    Q_FOREACH(WhiteWidget* iteratorWidget, whitewidgetList)
    {
        iteratorWidget->setOpacity(op);
    }
}

qreal WhiteWidgetManager::opacity() const
{
    return whitewidgetList.first()->opacity();
}

WhiteWidgetManager::~WhiteWidgetManager()
{
    qDeleteAll(whitewidgetList);
    whitewidgetList.clear();
}
