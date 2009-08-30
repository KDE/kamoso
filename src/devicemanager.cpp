/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
	foreach (Solid::Device device,
			Solid::Device::listFromType(Solid::DeviceInterface::Video, QString())) {
		addDevice(device);
	}
	//Connect to solid events to get new devices.

	connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(const QString&)), SLOT(deviceAdded(const QString &)) );
	connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(const QString&)), SLOT(deviceRemoved(const QString &)) );
}

void DeviceManager::addDevice(Solid::Device device)
{
	m_deviceList.append(Device(device));
}

void DeviceManager::deviceAdded(const QString &udi)
{
	Solid::Device device( udi );
	if(device.is<Solid::Video>())
	{
		qDebug() << "Adding new Device";
		addDevice(device);
		emit deviceRegistered(udi);
	}
}

int DeviceManager::numberOfDevices()
{
	return m_deviceList.size();
}

void DeviceManager::removeDevice(Solid::Device device)
{
	//TODO: implement remove device
	for(int x=0;x < m_deviceList.size();x++)
	{
		if(m_deviceList[x].getUdi() == device.udi())
		{
			m_deviceList.removeAt(x);
			break;
		}
	}
}
void DeviceManager::deviceRemoved(const QString &udi)
{
	for(int x=0;x < m_deviceList.size();x++)
	{
		if(m_deviceList[x].getUdi() == udi)
		{
			m_deviceList.removeAt(x);
			emit deviceUnregistered(udi);
			break;
		}
	}
}
const QList <Device> DeviceManager::devices()
{
	return m_deviceList;
}
DeviceManager* DeviceManager::self()
{
	if(s_instance == NULL)
	{
		s_instance = new DeviceManager();
	}
	return s_instance;
}