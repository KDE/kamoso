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

Device::Device(QObject* parent)
    : QObject(parent)
{
//     m_udi = device.udi();
//     m_description = device.product();
//     m_path = "/dev/video0";

//     const Solid::Video *solidVideoDevice = device.as<Solid::Video>();
//     if (solidVideoDevice)
//     {
//         QStringList protocols = solidVideoDevice->supportedProtocols();
//         if ( protocols.contains( "video4linux" ) )
//         {
//             QStringList drivers = solidVideoDevice->supportedDrivers( "video4linux" );
//             qDebug() << drivers;
//             if ( drivers.contains( "video4linux" ) )
//             {
//                 m_path = solidVideoDevice->driverHandle( "video4linux" ).toByteArray();
//             }
//         }
//     }

    m_config = KSharedConfig::openConfig("kamosoDevices");
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

void Device::setBrightness(int level)
{
    qDebug() << "New brightness " << level;
    if (level == brightness()) {
        return;
    }

    Q_EMIT brightnessChanged(level, brightness());
    m_config->group(m_udi).writeEntry("brightness",level);
    m_config->sync();
}

void Device::setContrast(int level)
{
    qDebug() << "New contrast " << level;
    if (level == contrast()) {
        return;
    }

    Q_EMIT contrastChanged(level, contrast());
    m_config->group(m_udi).writeEntry("contrast",level);
    m_config->sync();
}

void Device::setSaturation(int level)
{
    qDebug() << "New saturation " << level;
    if (level == saturation()) {
        return;
    }

    Q_EMIT saturationChanged(level, saturation());
    m_config->group(m_udi).writeEntry("saturation",level);
    m_config->sync();
}

void Device::setGamma(int level)
{
    qDebug() << "new gamma" << level;
    if (level == gamma()) {
        return;
    }

    Q_EMIT gammaChanged(level, gamma());
    m_config->group(m_udi).writeEntry("gamma",level);
    m_config->sync();
}

void Device::setHue(int level)
{
    qDebug() << "new hue" << level;
    if (level == hue()) {
        return;
    }

    Q_EMIT hueChanged(level, hue());
    m_config->group(m_udi).writeEntry("hue",level);
    m_config->sync();
}

int Device::brightness() const
{
    return m_config->group(m_udi).readEntry("brightness",0);
}

int Device::contrast() const
{
    return m_config->group(m_udi).readEntry("contrast",100);
}

int Device::saturation() const
{
    return m_config->group(m_udi).readEntry("saturation",100);
}

int Device::gamma() const
{
    return m_config->group(m_udi).readEntry("gamma",100);
}

int Device::hue() const
{
    return m_config->group(m_udi).readEntry("hue",0);
}

QString Device::udi() const
{
//     TODO
    return m_path;
}
