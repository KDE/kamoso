/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(Device* playingDevice READ playingDevice WRITE setPlayingDevice NOTIFY playingDeviceChanged)
    public:
        static DeviceManager* self();
        enum {
            ObjectId = Qt::UserRole+1
        };
        virtual QHash<int, QByteArray> roleNames() const override;

        Device* playingDevice() const;
        void setPlayingDevice(Device* device);
        bool hasDevices() const;

        virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        void deviceRemoved(_GstDevice *device);
        void deviceAdded(_GstDevice *device);

        Q_SCRIPTABLE Device* deviceAt(int i) const;

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
