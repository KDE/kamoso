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
#include <QDebug>

DeviceManager *DeviceManager::s_instance = NULL;

DeviceManager::DeviceManager()
{
    //Checking current connected devices
    QList <Solid::Device> deviceList = Solid::Device::listFromType(Solid::DeviceInterface::Video, QString());
    foreach (const Solid::Device &device, deviceList) {
        addDevice(device);
    }

    //Connect to solid events to get new devices.
    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(QString)), SLOT(deviceAdded(QString)) );
    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(QString)), SLOT(deviceRemoved(QString)) );
}

/*
*Public methods
*/
QList<Device> DeviceManager::devices() const
{
    return m_deviceList;
}

int DeviceManager::numberOfDevices() const
{
    return m_deviceList.size();
}

Device& DeviceManager::defaultDevice()
{
    return m_deviceList.first();
}

QString DeviceManager::defaultDevicePath() const
{
    return m_deviceList.first().path();
}

QString DeviceManager::defaultDeviceUdi() const
{
    return m_deviceList.first().udi();
}

Device& DeviceManager::playingDevice()
{
    return m_playingDevice;
}

QString DeviceManager::playingDeviceUdi() const
{
    return m_playingUdi;
}

QString DeviceManager::playingDevicePath() const
{
    return m_playingPath;
}

/*
*Private methods
*/
void DeviceManager::addDevice(const Solid::Device& device)
{
    m_deviceList.append(Device(&device));
}

void DeviceManager::removeDevice(const Solid::Device& device)
{
    QList <Device> ::iterator i;
    for(i = m_deviceList.begin(); i != m_deviceList.end(); ++i)
    {
        if(i->udi() == device.udi())
        {
            m_deviceList.erase(i);
            break;
        }
    }
}

/*
*QT Slots 
*/
void DeviceManager::deviceRemoved(const QString &udi)
{
    QList <Device> ::iterator i;
    for(i=m_deviceList.begin();i!=m_deviceList.end();++i)
    {
        if(i->udi() == udi)
        {
            m_deviceList.erase(i);
            emit deviceUnregistered(udi);
            break;
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
}

void DeviceManager::webcamPlaying(const QString &udi)
{
    foreach(const Device &device, m_deviceList) {
        if(device.udi() == udi) {
            m_playingDevice = device;
            m_playingUdi = udi;
            m_playingPath = m_playingDevice.path();
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
