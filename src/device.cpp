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

#include "device.h"
#include <KConfigGroup>
#include <QDebug>

QString structureValue(GstStructure* device, const char* key)
{
    auto x = gst_structure_get_value(device, key);
    if (!x)
        return {};
    return QString::fromUtf8(g_value_get_string(x));
}

QString udiFromProperties(GstStructure* deviceProperties)
{
    QString udi;
    gboolean udev_probed;
    if (gst_structure_get_boolean(deviceProperties, "udev-probed", &udev_probed) && !udev_probed)
        udi = structureValue(deviceProperties, "device.path");
    else
        udi = structureValue(deviceProperties, "sysfs.path");
    
    return udi;
}

//     for reference, the properties can be listed with:
//     gst-device-monitor-1.0 Video/Source
Device::Device(GstDevice *device, QObject* parent)
    : QObject(parent)
    , m_description(gst_device_get_display_name(device))
    , m_device(device)
{
    auto st = gst_device_get_properties(device);
    m_udi = udiFromProperties(st);
    m_path = structureValue(st, "device.path");
    gst_structure_free(st);
}

Device::~Device()
{}

void Device::reset()
{
    m_filters.clear();

    Q_EMIT filtersChanged(m_filters);
}

bool Device::isValid() const
{
    return !m_udi.isEmpty() && !m_path.isEmpty();
}

void Device::setFilters(const QString &newFilters)
{
    if (newFilters == m_filters) {
        return;
    }

    m_filters = newFilters;
    Q_EMIT filtersChanged(newFilters);
}

GstElement* Device::createElement()
{
    return gst_device_create_element(m_device, m_description.toUtf8().constData());
}
