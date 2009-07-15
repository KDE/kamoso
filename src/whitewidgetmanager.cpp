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

#include "whitewidgetmanager.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <KLocale>
#include <KWindowSystem>
#include <QApplication>
#include <QDebug>

/**
*This class create and manage 1 white widget per screen, creating an unified interface for all of them
*/
WhiteWidgetManager::WhiteWidgetManager(QWidget* parent) : QObject(parent)
{
	qDebug() << "WhiteWidgetManager:  " << "WhiteWidgetManager has been instanced";
	this->createWhiteWidgets();
}
void WhiteWidgetManager::createWhiteWidgets()
{
	qDebug() << "WhiteWidgetManager:  " << "Creating whiteWidgets";
	WhiteWidget *whiteWidget;
	QDesktopWidget *desktopInfo = qApp->desktop();
	
	qDebug() << "WhiteWidgetManager:  " << "Num of whidgets to be created: " << desktopInfo->numScreens();
	for(uchar x=0;x<desktopInfo->numScreens();x++)
	{
 		whiteWidget = new WhiteWidget;
 		whiteWidget->setGeometry(desktopInfo->screenGeometry(x));
		whitewidgetList.append(whiteWidget);
	}
}
void WhiteWidgetManager::showAll()
{
	WhiteWidget *iteratorWidget;
	foreach(iteratorWidget,whitewidgetList)
	{
		iteratorWidget->showFullScreen();
	}
}
void WhiteWidgetManager::hideAll()
{
	WhiteWidget *iteratorWidget;
	foreach(iteratorWidget,whitewidgetList)
	{
		iteratorWidget->hide();
		delete iteratorWidget;
	}
	this->createWhiteWidgets();
}
// void WhiteWidgetManager::
WhiteWidgetManager::~WhiteWidgetManager()
{
// 	qDeleteAll<>()
}