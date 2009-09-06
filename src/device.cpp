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
	m_description = device->product();

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

Device::~Device()
{}

void Device::setPlaying(bool play)
{
	m_playing = play;
}

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

