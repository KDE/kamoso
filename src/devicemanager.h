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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QtCore/QObject>
#include <QAbstractListModel>
#include "device.h"

class DeviceManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString playingDevice READ playingDeviceUdi WRITE setPlayingDevice NOTIFY playingDeviceChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    public:
        static DeviceManager* self();
        enum {
            Udi=Qt::UserRole+1
        };

        Device* playingDevice();
        QString playingDeviceUdi() const;
        QByteArray playingDevicePath() const;
        void setPlayingDevice(const QString& udi);
        bool hasDevices() const;

        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    public Q_SLOTS:
        void webcamPlaying(const QString &udi);
        void save();

    private Q_SLOTS:
        void deviceAdded(const QString &udi);
        void deviceRemoved(const QString &udi);

    Q_SIGNALS:
        void playingDeviceChanged();
        void deviceRegistered( const QString & udi );
        void deviceUnregistered( const QString & udi );
        void countChanged();
        void noDevices();

    private:
        DeviceManager();
        static DeviceManager* s_instance;
        void addDevice(const Solid::Device& device);
        void removeDevice(const Solid::Device& device);
        QList<Device*> m_deviceList;
        Device *m_playingDevice;
};

#endif // DEVICEMANAGER_H
