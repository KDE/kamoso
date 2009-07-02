/*
    videodevicevector.cpp  -  Kopete Video Device List Abstration

    Copyright (c) 2009 by Alan Jones <skyphyr@gmail.com>

    Kopete    (c) 2002-2009      by the Kopete developers  <kopete-devel@kde.org>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU General Public                   *
    * License as published by the Free Software Foundation; either          *
    * version 3 of the License, or (at your option) any later version.      *
    *                                                                       *
    * This program is distributed in the hope that it will be useful,       *
    * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
    * GNU General Public License for more details.                          *
    *                                                                       *
    * You should have received a copy of the GNU General Public License     *
    * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
    *************************************************************************
*/

#include "videodevicevector.h"

namespace Kopete {

namespace AV {

VideoDevice& VideoDeviceVector::operator[] (int i)
	{
	if (i>=size())
		{
		//we're outside the range so let's return a default camera not available VideoDevice
		return fallback;
		}
	
	//if we've made it here then we're within range
	return QVector<VideoDevice>::operator[](i);
	}
	
const VideoDevice& VideoDeviceVector::operator[] (int i) const
	{
	return at(i);
	}

const VideoDevice& VideoDeviceVector::at(int i) const
	{
	if (i>=size())
		{
		//we're outside the range so let's return a default camera not available VideoDevice
		return fallback;
		}
	
	//if we've made it here then we're within range
	return QVector<VideoDevice>::operator[](i);
	}

}

}
