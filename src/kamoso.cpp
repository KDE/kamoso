/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
 *  Copyright (C) 2013      by Michael G. Hansen <mike@mghansen.de>                  *
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


#include "kamoso.h"

#include "kamosoSettings.h"
#include "video/webcamcontrol.h"
#include "devicemanager.h"
#include <KIO/Global>
#include <KIO/CopyJob>

#include <QtCore/QFile>

Kamoso::Kamoso(WebcamControl *webcamControl)
{
    m_webcamControl = webcamControl;
    connect(m_webcamControl, SIGNAL(photoTaken(QString)), this, SIGNAL(photoTaken(QString)));
}

Kamoso::~Kamoso()
{

}

const QString Kamoso::takePhoto()
{
    const QUrl saveUrl = Settings::saveUrl();
    const QString initialName = QStringLiteral("picture.jpg");
    QUrl path = saveUrl.toString() + '/' + initialName;

    if (path.isLocalFile() && QFile::exists(path.toLocalFile())) {
        path = saveUrl.toString() + '/' + KIO::suggestName(saveUrl, initialName);
    }

    m_webcamControl->takePhoto(path);

    return path.toDisplayString();
}

void Kamoso::startRecording()
{
    m_webcamControl->startRecording();
}

void Kamoso::stopRecording()
{
    const QUrl saveUrl = Settings::saveUrl();
    const QString initialName = QStringLiteral("video.mkv");

    QUrl path = saveUrl.toString() + '/' + initialName;

    if (path.isLocalFile() && QFile::exists(path.toLocalFile())) {
        path = saveUrl.toString() + '/' + KIO::suggestName(saveUrl, initialName);
    }

    KJob *job = KIO::move(QUrl::fromLocalFile(m_webcamControl->stopRecording()), path);
    job->start();

    m_webcamControl->play(DeviceManager::self()->playingDevice());
}

void Kamoso::resetDeviceSettings()
{
    Device *device = DeviceManager::self()->playingDevice();
    device->reset();
}
