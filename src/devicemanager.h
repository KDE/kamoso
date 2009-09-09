/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2009 by Alex Fiestas <alex@eyeos.org>                         *
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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QtCore/QObject>
#include "device.h"

class DeviceManager : public QObject
{
Q_OBJECT
public:
	static DeviceManager* self();
	int numberOfDevices() const;
	QList<Device> devices() const;
	QString defaultDevicePath() const;
	QString defaultDeviceUdi() const;
	QString playingDeviceUdi() const;
	QString playingDevicePath() const;
	
public slots:
	void webcamPlaying(const QString &udi);
	
private slots:
	void deviceAdded(const QString &udi);
	void deviceRemoved(const QString &udi);
	
signals:
	void deviceRegistered( const QString & udi );
	void deviceUnregistered( const QString & udi );
	
private:
	DeviceManager();
	static DeviceManager* s_instance;
	void addDevice(const Solid::Device& device);
	void removeDevice(const Solid::Device& device);
	QList<Device> m_deviceList;
	QString m_playingUdi;
	QString m_playingPath;
};

#endif // DEVICEMANAGER_H
