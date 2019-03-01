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

#include <QObject>
#include <QAbstractListModel>
#include "device.h"

struct _GstDevice;
struct _GstDeviceMonitor;

class DeviceManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString playingDeviceUdi READ playingDeviceUdi WRITE setPlayingDeviceUdi NOTIFY playingDeviceChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(Device* playingDevice READ playingDevice NOTIFY playingDeviceChanged)
    public:
        static DeviceManager* self();
        enum {
            Udi=Qt::UserRole+1
        };
        virtual QHash<int, QByteArray> roleNames() const override;

        Device* playingDevice();
        QString playingDevicePath() const;
        QString playingDeviceUdi() const;
        void setPlayingDeviceUdi(const QString& path);
        bool hasDevices() const;

        virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        void deviceRemoved(_GstDevice *device);
        void deviceAdded(_GstDevice *device);

        Q_SCRIPTABLE QString udiAt(int i) const;

    public Q_SLOTS:
        void save();

    Q_SIGNALS:
        void playingDeviceChanged();
        void countChanged();
        void noDevices();

    private:
        DeviceManager();
        ~DeviceManager() override;
        static DeviceManager* s_instance;

        QVector<Device*> m_deviceList;
        Device *m_playingDevice;
        _GstDeviceMonitor *m_monitor;
};

#endif // DEVICEMANAGER_H
