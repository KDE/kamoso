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

#include "photoshootmode.h"
#include "kamoso.h"
#include "countdownwidget.h"

#include <KIcon>
#include <KLocalizedString>

#include <QAction>
#include <QPushButton>
#include "devicemanager.h"

PhotoShootMode::PhotoShootMode(Kamoso* camera)
    : ShootMode(camera)
{
    QAction* flash=new QAction(KIcon("weather-clear"), i18n("Use Flash"), this);
    flash->setCheckable(true);
    flash->setChecked(controller()->isFlashEnabled());
    connect(flash, SIGNAL(triggered(bool)), controller(), SLOT(setFlashEnabled(bool)));
    mActions += flash;
}

void PhotoShootMode::deactivate()
{
    controller()->stopCountdown();
    disconnect(controller()->countdown(), SIGNAL(finished()),this ,SLOT(release()));
}

QWidget* PhotoShootMode::mainAction()
{
    mTrigger = new QPushButton(controller());
    mTrigger->setIcon(icon());
    mTrigger->setIconSize(QSize(32,32));
    mTrigger->setToolTip(name());
    mTrigger->setCheckable(true);
    mTrigger->setShortcut(Qt::Key_WebCam);

    connect(controller()->countdown(), SIGNAL(finished()), SLOT(release()));
    connect(mTrigger, SIGNAL(clicked(bool)), this, SLOT(shootClicked(bool)));
    return mTrigger;
}

void PhotoShootMode::release()
{
    mTrigger->setChecked(false);
}

void PhotoShootMode::shootClicked(bool pressed)
{
    if(pressed && DeviceManager::self()->hasDevices()) {
        controller()->startCountdown();
    } else {
        controller()->stopCountdown();
    }
}

QIcon PhotoShootMode::icon() const
{
    return KIcon("camera-web");
}

QString PhotoShootMode::name() const
{
    return i18n("Take pictures");
}

QStringList PhotoShootMode::thumbnailsViewMimeTypes() const
{
    return QStringList() << "image/png";
}
