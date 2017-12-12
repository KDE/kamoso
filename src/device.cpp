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

//     for refrence, the properties can be listed with:
//     gst-device-monitor-1.0 Video/Source
Device::Device(const QGst::Structure &device, QObject* parent)
    : QObject(parent)
    , m_description(device.value("device.product.name").toString())
    , m_udi(device.value("sysfs.path").toString())
    , m_path(device.value("device.path").toString())
{
    qDebug() << "new device" << m_description << m_udi << m_path;
}

Device::~Device()
{}

void Device::reset()
{
    m_filters.clear();

    Q_EMIT filtersChanged(m_filters);
}

void Device::setFilters(const QString &newFilters)
{
    if (newFilters == m_filters) {
        return;
    }

    m_filters = newFilters;
    Q_EMIT filtersChanged(newFilters);
}
