
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

