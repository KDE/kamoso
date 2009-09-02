/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@gmail.com>                        *
 *  Copyright (C) 2008-2009 by Alex Fiestas <alex@eyeos.org>                         *
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
#include <QDebug>
Device::Device(const Solid::Device *device)
{
	m_udi = device->udi();
	m_description = device->description();

	const Solid::Video *solidVideoDevice = device->as<Solid::Video>();
	if (solidVideoDevice)
	{
		QStringList protocols = solidVideoDevice->supportedProtocols();
		if ( protocols.contains( "video4linux" ) )
		{
			QStringList drivers = solidVideoDevice->supportedDrivers( "video4linux" );
			if ( drivers.contains( "video4linux" ) )
			{
				m_path = solidVideoDevice->driverHandle( "video4linux" ).toString();
			}
		}
	}
}

void Device::playing(bool play)
{
	m_playing = play;
}

QString Device::path()
{
	return m_path;
}

QString Device::description()
{
	return m_description;
}

QString Device::udi()
{
	return m_udi;
}

Device::~Device()
{

}

