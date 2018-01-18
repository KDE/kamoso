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

    if (devices == NULL) {
        qWarning ("No device found");
    }

    /* Initialize camera structures */
    while(devices) {
        deviceAdded(GST_DEVICE(devices->data));
        devices = devices->next;
    }

    g_list_free (devices);

    if (!m_deviceList.isEmpty()) {
        setPlayingDeviceUdi(m_deviceList.first()->udi());
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
    roles.insert(Udi, "udi");
    return roles;
}

void DeviceManager::save()
{
    if (!m_playingDevice) {
        return;
    }

    Settings::self()->setDeviceUdi(m_playingDevice->udi());
}

/*
*Public methods
*/
int DeviceManager::rowCount(const QModelIndex& idx) const
{
    return idx.isValid() ? 0 : m_deviceList.size();
}

void DeviceManager::setPlayingDeviceUdi(const QString& udi)
{
    Q_FOREACH(Device* d, m_deviceList) {
        if(d->udi() == udi) {
            if (m_playingDevice != d) {
                m_playingDevice = d;
                qDebug() << "Playing device changed" << d->path();
                Q_EMIT playingDeviceChanged();
            }
            return;
        }
    }

    m_playingDevice = 0;
}

Device* DeviceManager::playingDevice()
{
    return m_playingDevice;
}

QString DeviceManager::playingDeviceUdi() const
{
    if (!m_playingDevice) {
        return QString();
    }

    return m_playingDevice->udi();
}

QString DeviceManager::playingDevicePath() const
{
    if (!m_playingDevice) {
        return QByteArray();
    }

    return m_playingDevice->path();
}

QVariant DeviceManager::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(!index.isValid() || row < 0 || row>=m_deviceList.size())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
            return m_deviceList[row]->description();
        case Udi:
            return m_deviceList[row]->udi();
    }
    return QVariant();
}

void DeviceManager::deviceAdded(GstDevice* device)
{
    auto st= gst_device_get_properties(device);

    const int s = m_deviceList.size();
    beginInsertRows({}, s, s);
    m_deviceList.append(new Device(st, this));
    endInsertRows();
}

void DeviceManager::deviceRemoved(GstDevice* device)
{
    auto st(gst_device_get_properties(device));
    auto udi = structureValue(st, "sysfs.path");

    for(int i = 0, c = m_deviceList.size(); i<c; ++i) {
        auto dev = m_deviceList.at(i);
        if (m_playingDevice == dev) {
            m_playingDevice = nullptr;
            Q_EMIT playingDeviceChanged();
        }

        if (dev->udi() == udi) {
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
