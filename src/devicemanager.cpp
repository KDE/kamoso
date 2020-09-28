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
#include "kamosoSettings.h"
#include <QDebug>

#include <gst/gststructure.h>
#include <gst/gstdevice.h>
#include <gst/gstdevicemonitor.h>
#include <gst/gstbus.h>

DeviceManager *DeviceManager::s_instance = NULL;

static gboolean
deviceMonitorWatch(GstBus     */*bus*/, GstMessage *message, gpointer /*user_data*/)
{
    GstDevice *device;
    switch (GST_MESSAGE_TYPE (message))
    {
        case GST_MESSAGE_DEVICE_ADDED:
            gst_message_parse_device_added (message, &device);
            DeviceManager::self()->deviceAdded(device);
            break;
        case GST_MESSAGE_DEVICE_REMOVED:
            gst_message_parse_device_removed (message, &device);
            DeviceManager::self()->deviceRemoved(device);
            break;
        default:
            break;
    }
    return G_SOURCE_CONTINUE;
}

DeviceManager::DeviceManager() : m_playingDevice(0)
{
    m_monitor = gst_device_monitor_new();

    GstBus *bus = gst_device_monitor_get_bus (m_monitor);
    gst_bus_add_watch (bus, deviceMonitorWatch, m_monitor);
    gst_object_unref (bus);

    GstCaps *caps = gst_caps_new_empty_simple ("video/x-raw");
    gst_device_monitor_add_filter (m_monitor, "Video/Source", caps);
    gst_caps_unref (caps);

    gst_device_monitor_start (m_monitor);

    GList* devices = gst_device_monitor_get_devices (m_monitor);
    if (!devices) {
        qWarning ("No device found");
    }

    /* Initialize camera structures */
    for(; devices; devices = devices->next) {
        auto device = new Device(GST_DEVICE(devices->data), this);
        m_deviceList.append(device);
    }

    g_list_free (devices);

    if (!m_deviceList.isEmpty()) {
        setPlayingObjectId(m_deviceList.constFirst()->objectId());
    }
}

DeviceManager::~DeviceManager()
{
    gst_device_monitor_stop(m_monitor);
    g_clear_object (&m_monitor);
}

QHash<int, QByteArray> DeviceManager::roleNames() const
{
    QHash< int, QByteArray > roles = QAbstractListModel::roleNames();
    roles.insert(ObjectId, "objectId");
    return roles;
}

void DeviceManager::save()
{
    if (!m_playingDevice) {
        return;
    }

    Settings::self()->setDeviceObjectId(m_playingDevice->objectId());
}

/*
*Public methods
*/
int DeviceManager::rowCount(const QModelIndex& idx) const
{
    return idx.isValid() ? 0 : m_deviceList.size();
}

void DeviceManager::setPlayingObjectId(const QString &objectId)
{
    Q_FOREACH(Device* d, m_deviceList) {
        if(d->objectId() == objectId) {
            if (m_playingDevice != d) {
                m_playingDevice = d;
                qDebug() << "Playing device changed" << d->description();
                Q_EMIT playingDeviceChanged();
            }
            return;
        }
    }

    qWarning() << "could not find device" << objectId;
    m_playingDevice = nullptr;
}

Device* DeviceManager::playingDevice()
{
    return m_playingDevice;
}

QString DeviceManager::playingObjectId() const
{
    if (!m_playingDevice) {
        return {};
    }

    return m_playingDevice->objectId();
}

QString DeviceManager::objectIdAt(int i) const
{
    return m_deviceList[i]->objectId();
}

QVariant DeviceManager::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(!index.isValid() || row < 0 || row>=m_deviceList.size())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
            return m_deviceList[row]->description();
        case ObjectId:
            return m_deviceList[row]->objectId();
    }
    return QVariant();
}

void DeviceManager::deviceAdded(GstDevice* device)
{
    const int s = m_deviceList.size();
    auto d = new Device(device, this);
    for (auto device : qAsConst(m_deviceList)) {
        if (device->objectId() == d->objectId()) {
            delete d;
            return;
        }
    }

    beginInsertRows({}, s, s);
    m_deviceList.append(d);
    endInsertRows();

    if (!m_playingDevice) {
        setPlayingObjectId(m_deviceList.first()->objectId());
    }
}

void DeviceManager::deviceRemoved(GstDevice* device)
{
    auto st = gst_device_get_properties(device);
    auto objectId = objectIdFromProperties(st);

    for(int i = 0, c = m_deviceList.size(); i<c; ++i) {
        auto dev = m_deviceList.at(i);
        if (m_playingDevice == dev) {
            m_playingDevice = nullptr;
            Q_EMIT playingDeviceChanged();
        }

        if (dev->objectId() == objectId) {
            beginRemoveRows({}, i, i);
            dev->deleteLater();
            m_deviceList.removeAt(i);
            endRemoveRows();
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
