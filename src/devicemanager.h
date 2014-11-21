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

namespace UdevQt{
    class Device;
    class Client;
}
class DeviceManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString playingDeviceUdi READ playingDeviceUdi WRITE setPlayingDeviceUdi NOTIFY playingDeviceUdiChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(Device* playingDevice READ playingDevice NOTIFY playingDeviceChanged)
    public:
        static DeviceManager* self();
        enum {
            Udi=Qt::UserRole+1
        };

        Device* playingDevice();
        QString playingDeviceUdi() const;
        QByteArray playingDevicePath() const;
        void setPlayingDeviceUdi(const QString& udi);
        bool hasDevices() const;

        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        QString udi() const;

    public Q_SLOTS:
        void webcamPlaying(const QString &udi);
        void save();
        void deviceRemoved(const UdevQt::Device &device);
        void deviceAdded(const UdevQt::Device &device);

    Q_SIGNALS:
        void playingDeviceChanged();
        void playingDeviceUdiChanged();
        void deviceRegistered( const QString & udi );
        void deviceUnregistered( const QString & udi );
        void countChanged();
        void noDevices();

    private:
        DeviceManager();
        static DeviceManager* s_instance;
        UdevQt::Client *m_client;

        QList<Device*> m_deviceList;
        Device *m_playingDevice;
};

#endif // DEVICEMANAGER_H
