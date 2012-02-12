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
#include <solid/video.h>
#include <KConfigGroup>
#include <kdebug.h>

Device::Device()
{}

Device::Device(const Solid::Device *device)
{
    m_udi = device->udi();
    m_description = device->product();

    const Solid::Video *solidVideoDevice = device->as<Solid::Video>();
    if (solidVideoDevice)
    {
        QStringList protocols = solidVideoDevice->supportedProtocols();
        if ( protocols.contains( "video4linux" ) )
        {
            QStringList drivers = solidVideoDevice->supportedDrivers( "video4linux" );
            kDebug() << drivers;
            if ( drivers.contains( "video4linux" ) )
            {
                m_path = solidVideoDevice->driverHandle( "video4linux" ).toString();
            }
        }
    }

    config = new KConfig("kamosoDevices");
}

Device::~Device()
{}

QString Device::path() const
{
    return m_path;
}

QString Device::description() const
{
    return m_description;
}

QString Device::udi() const
{
    return m_udi;
}

QString Device::vendor() const
{
    return m_vendor;
}

void Device::setBrightness(int level)
{
    kDebug() << "New brightness " << level;
    config->group(m_udi).writeEntry("brightness",level);
    config->sync();
}

void Device::setContrast(int level)
{
    kDebug() << "New contrast " << level;
    config->group(m_udi).writeEntry("contrast",level);
    config->sync();
}

void Device::setSaturation(int level)
{
    kDebug() << "New saturation " << level;
    config->group(m_udi).writeEntry("saturation",level);
    config->sync();
}

void Device::setGamma(int level)
{
    kDebug() << "new gamma" << level;
    config->group(m_udi).writeEntry("gamma",level);
    config->sync();
}

void Device::setHue(int level)
{
    kDebug() << "new hue" << level;
    config->group(m_udi).writeEntry("hue",level);
    config->sync();
}

int Device::brightness() const
{
    return config->group(m_udi).readEntry("brightness",0);
}

int Device::contrast() const
{
    return config->group(m_udi).readEntry("contrast",100);
}

int Device::saturation() const
{
    return config->group(m_udi).readEntry("saturation",100);
}

int Device::gamma() const
{
    return config->group(m_udi).readEntry("gamma",100);
}

int Device::hue() const
{
    return config->group(m_udi).readEntry("hue",0);
}
