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


#include "videoshootmode.h"
#include "devicemanager.h"

#include <QPushButton>
#include <KIcon>
#include <KLocalizedString>
#include "kamoso.h"
#include <QAction>

VideoShootMode::VideoShootMode(Kamoso* camera)
    : ShootMode(camera)
{
    QAction* sound=new QAction(KIcon("audio-input-microphone"), i18n("Record Audio"), this);
    sound->setCheckable(true);
    sound->setChecked(true);

    mActions += sound;
}

void VideoShootMode::deactivate()
{
    controller()->stopVideo();
}

QWidget* VideoShootMode::mainAction()
{
    QPushButton* action = new QPushButton(controller());
    action->setIcon(icon());
    action->setIconSize(QSize(32,32));
    action->setToolTip(name());
    action->setCheckable(true);
    action->setShortcut(Qt::Key_WebCam);

    #warning TODO
    connect(action, SIGNAL(clicked(bool)), this, SLOT(videoPressed(bool)));
    return action;
}

void VideoShootMode::videoPressed(bool pressed)
{
    if(pressed && DeviceManager::self()->hasDevices()) {
        controller()->startVideo(mActions.first()->isChecked());
    } else {
        controller()->stopVideo();
    }
}

QIcon VideoShootMode::icon() const
{
    return KIcon("media-record");
}

QString VideoShootMode::name() const
{
    return i18n("Take a video");
}

QStringList VideoShootMode::thumbnailsViewMimeTypes() const
{
    return QStringList() << "video/x-matroska";
}
