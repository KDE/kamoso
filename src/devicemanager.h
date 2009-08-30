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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QtCore/QObject>
#include "device.h"

class DeviceManager : public QObject
{
Q_OBJECT
public:
	static DeviceManager* self();
	int numberOfDevices();
	const QList <Device> devices();
private:
	DeviceManager();
	static DeviceManager* s_instance;
	void addDevice(Solid::Device device);
	void removeDevice(Solid::Device device);
	QList<Device> m_deviceList;
private slots:
	void deviceAdded(const QString &udi);
	void deviceRemoved(const QString &udi);
signals:
	void deviceRegistered( const QString & udi );
	void deviceUnregistered( const QString & udi );
};

#endif // DEVICEMANAGER_H
