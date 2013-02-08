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
#include <solid/device.h>
#include <solid/devicenotifier.h>
#include <solid/deviceinterface.h>
#include <solid/video.h>

#include "device.h"
#include <settings.h>
#include <QDebug>

DeviceManager *DeviceManager::s_instance = NULL;

DeviceManager::DeviceManager()
{
    QHash< int, QByteArray > roles=roleNames();
    roles.insert(Udi, "udi");
    setRoleNames(roles);

    //Connect to solid events to get new devices.
    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(const QString&)), SLOT(deviceAdded(const QString &)) );
    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(const QString&)), SLOT(deviceRemoved(const QString &)) );

    //Checking current connected devices
    QList <Solid::Device> deviceList = Solid::Device::listFromType(Solid::DeviceInterface::Video, QString());
    if (deviceList.isEmpty()) {
        qDebug() << "No devices fount";
        return;
    }

    foreach (const Solid::Device &device, deviceList) {
        addDevice(device);
    }

    QString udi = Settings::self()->deviceUdi();
    foreach(const Device& d, m_deviceList) {
        if(d.udi()==udi) {
            m_playingDevice = d;
        }
    }

    if(m_playingDevice.path().isEmpty())
        m_playingDevice = m_deviceList.first();
}

void DeviceManager::save()
{
    Settings::self()->setDeviceUdi(m_playingDevice.udi());
}

/*
*Public methods
*/
int DeviceManager::rowCount(const QModelIndex& ) const
{
    return m_deviceList.size();
}

void DeviceManager::setPlayingDevice(const QString& udi)
{
    foreach(const Device& d, m_deviceList) {
        if(d.udi()==udi) {
            m_playingDevice=d;
            qDebug() << "Playing device changed";
            emit playingDeviceChanged();
            return;
        }
    }

    m_playingDevice = Device();
}

Device& DeviceManager::playingDevice()
{
    return m_playingDevice;
}

QString DeviceManager::playingDeviceUdi() const
{
    return m_playingDevice.udi();
}

QByteArray DeviceManager::playingDevicePath() const
{
    return m_playingDevice.path();
}

QVariant DeviceManager::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(!index.isValid() || row < 0 || row>=m_deviceList.size())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
            return m_deviceList[row].description();
        case Udi:
            return m_deviceList[row].udi();
    }
    return QVariant();
}

/*
*Private methods
*/
void DeviceManager::addDevice(const Solid::Device& device)
{
    beginInsertRows(QModelIndex(), m_deviceList.count(), m_deviceList.count());
    m_deviceList.append(Device(&device));
    endInsertRows();
    emit countChanged();
}

void DeviceManager::removeDevice(const Solid::Device& device)
{
    deviceRemoved(device.udi());
}

/*
*QT Slots 
*/
void DeviceManager::deviceRemoved(const QString &udi)
{
    for(int i=0; i<m_deviceList.count(); ++i)
    {
        if(m_deviceList[i].udi() == udi)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_deviceList.removeAt(i);
            endRemoveRows();
            emit countChanged();
            break;
        }
    }

    if(udi==m_playingDevice.udi()) {
        if(m_deviceList.isEmpty()) {
            emit noDevices();
            setPlayingDevice(QString());
        } else {
            setPlayingDevice(m_deviceList.first().udi());
        }
    }
}

void DeviceManager::deviceAdded(const QString &udi)
{
    Solid::Device device( udi );
    if(device.is<Solid::Video>())
    {
        addDevice(device);
        emit deviceRegistered(udi);
    }

    if (m_playingDevice.udi().isEmpty()) {
        qDebug() << "Playing  added device";
        setPlayingDevice(udi);
    }
}

void DeviceManager::webcamPlaying(const QString &udi)
{
    foreach(const Device &device, m_deviceList) {
        if(device.udi() == udi) {
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