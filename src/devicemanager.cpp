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

#include "device.h"
#include <settings.h>
#include <QDebug>
#include <gstreamer-1.0/gst/gstdevicemonitor.h>

DeviceManager *DeviceManager::s_instance = NULL;

static gboolean
my_bus_func (GstBus * bus, GstMessage * message, gpointer user_data)
{
    GstDevice *device;
    gchar* name;

    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_DEVICE_ADDED:
            gst_message_parse_device_added (message, &device);
            name = gst_device_get_display_name (device);
            qDebug() << "Device added:" << name;
            g_free (name);
            break;
        case GST_MESSAGE_DEVICE_REMOVED:
            gst_message_parse_device_removed (message, &device);
            name = gst_device_get_display_name (device);
            qDebug() << "Device removed:" << name;
            g_free (name);
            break;
        default:
            break;
    }

    return G_SOURCE_CONTINUE;
}

GstDeviceMonitor *
setup_raw_video_source_device_monitor (void) {
    GstDeviceMonitor *monitor;
    GstBus *bus;
    GstCaps *caps;

    monitor = gst_device_monitor_new ();

    bus = gst_device_monitor_get_bus (monitor);
    gst_bus_add_watch (bus, my_bus_func, NULL);
    gst_object_unref (bus);

    caps = gst_caps_new_empty_simple ("video/x-raw");
    gst_device_monitor_add_filter (monitor, "Video/Source", caps);
    gst_caps_unref (caps);

    gst_device_monitor_start (monitor);

    return monitor;
}

DeviceManager::DeviceManager() : m_playingDevice(0)
{
    QHash< int, QByteArray > roles=roleNames();
    roles.insert(Udi, "udi");
    setRoleNames(roles);


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
int DeviceManager::rowCount(const QModelIndex& ) const
{
    return m_deviceList.size();
}

void DeviceManager::setPlayingDeviceUdi(const QString& udi)
{
    foreach(Device* d, m_deviceList) {
        if(d->udi() == udi) {
            m_playingDevice = d;
            qDebug() << "Playing device changed";
            emit playingDeviceUdiChanged();
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

QByteArray DeviceManager::playingDevicePath() const
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

/*
*Private methods
*/
// void DeviceManager::addDevice(const Solid::Device& device)
// {
//     beginInsertRows(QModelIndex(), m_deviceList.count(), m_deviceList.count());
//     m_deviceList.append(new Device(device));
//     endInsertRows();
//     emit countChanged();
// }
//
// void DeviceManager::removeDevice(const Solid::Device& device)
// {
//     deviceRemoved(device.udi());
// }

/*
*QT Slots 
*/
// void DeviceManager::deviceRemoved(const QString &udi)
// {
//     for(int i=0; i<m_deviceList.count(); ++i)
//     {
//         if(m_deviceList[i]->udi() == udi)
//         {
//             beginRemoveRows(QModelIndex(), i, i);
//             m_deviceList.removeAt(i);
//             endRemoveRows();
//             emit countChanged();
//             break;
//         }
//     }
//
//     if(udi == m_playingDevice->udi()) {
//         if(m_deviceList.isEmpty()) {
//             emit noDevices();
//             setPlayingDeviceUdi(QString());
//         } else {
//             setPlayingDeviceUdi(m_deviceList.first()->udi());
//         }
//     }
// }

void DeviceManager::webcamPlaying(const QString &udi)
{
    foreach(Device *device, m_deviceList) {
        if(device->udi() == udi) {
            m_playingDevice = device;
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
