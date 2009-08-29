
#include "device.h"
#include <QDebug>
Device::Device(Solid::Device device)
{
	//Saving the new device locally
// 	m_device = device;
	//v4l2 properties are not queried until they're needed. 
	//So at the moment we're only going to save the Solid:Device information.
// 	setDescription(device.description());
	m_udi = device.udi();
	m_description = device.description();
}

QString Device::getDescription()
{
	return m_description;
}

QString Device::getUdi()
{
	return m_udi;
}

Device::~Device()
{

}

