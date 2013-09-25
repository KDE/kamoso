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
#include <QDesktopWidget>
#include <QApplication>

#include <KDebug>
#include <KLocale>

/**
*This class create and manage 1 white widget per screen, creating an unified interface for all of them
*/
WhiteWidgetManager::WhiteWidgetManager(QWidget* parent) : QObject(parent)
{
    kDebug() << "WhiteWidgetManager has been instanced";
    createWhiteWidgets();

    //Call tick each 30 ms on showAll call
    m_timer = new QTimer(this);
    m_currentStep = 0;

    //Maybe we should set it as cons again, but at the moment I'd like the idea to have it variable
    //I've also moved it to public scope
    m_steps = 10;

    connect(m_timer, SIGNAL(timeout()), SLOT(tick()));
}

/**
*This method detect the current screens and create one whiteWidget per screen
*/
void WhiteWidgetManager::createWhiteWidgets()
{
    kDebug() << "Creating whiteWidgets";
    WhiteWidget *whiteWidget;
    QDesktopWidget *desktopInfo = qApp->desktop();

    kDebug() << "Num of whidgets to be created: " << desktopInfo->numScreens();
    for(uchar x=0;x<desktopInfo->numScreens();++x)
    {
        whiteWidget = new WhiteWidget;
        whiteWidget->setGeometry(desktopInfo->screenGeometry(x));
        whitewidgetList.append(whiteWidget);
    }
}

/**
*This method show all whiteWidgets, and start the unified Qtimer
*/
void WhiteWidgetManager::showAll()
{
    WhiteWidget *iteratorWidget;
    m_timer->start(30);

    foreach(iteratorWidget,whitewidgetList)
    {
        iteratorWidget->showFullScreen();
    }
}

/**
*This method is usually called by timeout, and hide all the whiteWidgets 
*/
void WhiteWidgetManager::hideAll()
{
    WhiteWidget *iteratorWidget;
    foreach(iteratorWidget,whitewidgetList)
    {
        iteratorWidget->hide();
    }
}

/**
*This slot is called each time that Qtimer each timeout (each 30s)
*/
void WhiteWidgetManager::tick()
{
    WhiteWidget *iteratorWidget;
    m_currentStep=qMin(m_currentStep+1, m_steps);

    foreach(iteratorWidget,whitewidgetList)
    {
        iteratorWidget->setWindowOpacity(m_currentStep);
    }
}

WhiteWidgetManager::~WhiteWidgetManager()
{
    qDeleteAll(whitewidgetList.begin(),whitewidgetList.end());
    whitewidgetList.clear();
}
