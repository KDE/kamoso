/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    if (qApp->platformName() != QLatin1String("wayland")) {
        m_timer->setPropertyName("opacity");
    }
    connect(m_timer, &QPropertyAnimation::finished, this, &WhiteWidgetManager::hideAll);
}

/**
*This method detect the current screens and create one whiteWidget per screen
*/
void WhiteWidgetManager::createWhiteWidgets()
{
    QList<QScreen*> screens = qGuiApp->screens();



    for (QScreen* screen: screens)
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

    for (WhiteWidget *iteratorWidget: whitewidgetList)
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
    for (WhiteWidget *iteratorWidget:  whitewidgetList)
    {
        iteratorWidget->hide();
    }
    m_timer->stop();
}

void WhiteWidgetManager::setOpacity(qreal op)
{
    for (WhiteWidget* iteratorWidget: whitewidgetList)
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
