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
#include "udev/udevqt.h"
#include <KConfigGroup>
#include <QDebug>

//     for refrence, the properties can be listed with:
//     udevadm info --query=property --name=/dev/video0
Device::Device(const UdevQt::Device &device, QObject* parent)
    : QObject(parent)
    , m_description(device.deviceProperty("ID_MODEL").toString().replace('_', ' '))
    , m_udi(device.sysfsPath())
    , m_path(device.deviceProperty("DEVNAME").toString().toLatin1())
{
}

Device::~Device()
{}

QByteArray Device::path() const
{
    return m_path;
}

QString Device::description() const
{
    return m_description;
}

QString Device::vendor() const
{
    return m_vendor;
}

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

QString Device::filters() const
{
    return m_filters;
}

QString Device::udi() const
{
    return m_path;
}
