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

#include "devicemanager.h"

#include "device.h"
#include "udev/udevqt.h"
#include <settings.h>
#include <QDebug>

DeviceManager *DeviceManager::s_instance = NULL;

DeviceManager::DeviceManager() : m_playingDevice(0)
{
    QHash< int, QByteArray > roles = roleNames();
    roles.insert(Udi, "udi");
    setRoleNames(roles);

    QStringList subsystems;
    subsystems << "video4linux";
    m_client = new UdevQt::Client(subsystems, this);
    connect(m_client, SIGNAL(deviceAdded(UdevQt::Device)), SLOT(deviceAdded(UdevQt::Device)));
    connect(m_client, SIGNAL(deviceRemoved(UdevQt::Device)), SLOT(deviceRemoved(UdevQt::Device)));

    const UdevQt::DeviceList deviceList = m_client->devicesBySubsystem("video4linux");

    Q_FOREACH(const UdevQt::Device &device, deviceList) {
        m_deviceList.append(new Device(device));
    }

    if (!m_deviceList.isEmpty()) {
        setPlayingDeviceUdi(m_deviceList.first()->udi());
    }
}

void DeviceManager::save()
{
    if (!m_playingDevice) {
        return;
    }

    Settings::self()->setDeviceUdi(m_playingDevice->udi());
}

/*
*Public methods
*/
int DeviceManager::rowCount(const QModelIndex& ) const
{
    return m_deviceList.size();
}

void DeviceManager::setPlayingDeviceUdi(const QString& udi)
{
    foreach(Device* d, m_deviceList) {
        if(d->udi() == udi) {
            m_playingDevice = d;
            qDebug() << "Playing device changed";
            emit playingDeviceUdiChanged();
            return;
        }
    }

    m_playingDevice = 0;
}

Device* DeviceManager::playingDevice()
{
    return m_playingDevice;
}

QString DeviceManager::playingDeviceUdi() const
{
    if (!m_playingDevice) {
        return QString();
    }

    return m_playingDevice->udi();
}

QByteArray DeviceManager::playingDevicePath() const
{
    if (!m_playingDevice) {
        return QByteArray();
    }

    return m_playingDevice->path();
}

QVariant DeviceManager::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(!index.isValid() || row < 0 || row>=m_deviceList.size())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
            return m_deviceList[row]->description();
        case Udi:
            return m_deviceList[row]->udi();
    }
    return QVariant();
}

void DeviceManager::deviceAdded(const UdevQt::Device& device)
{
    m_deviceList.append(new Device(device));
}

void DeviceManager::deviceRemoved(const UdevQt::Device& device)
{
    Q_FOREACH(Device *mDevice, m_deviceList) {
        if (mDevice->udi() == device.sysfsPath()) {
            m_deviceList.removeAll(mDevice);
        }
    }
}

void DeviceManager::webcamPlaying(const QString &udi)
{
    foreach(Device *device, m_deviceList) {
        if(device->udi() == udi) {
            m_playingDevice = device;
            break;
        }
    }
}

bool DeviceManager::hasDevices() const
{
    return !m_deviceList.isEmpty();
}

/*
*Singleton
*/
DeviceManager* DeviceManager::self()
{
    if(s_instance == NULL)
    {
        s_instance = new DeviceManager();
    }
    return s_instance;
}
