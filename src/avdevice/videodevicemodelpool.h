/*
    videodevicepool.h  -  Kopete Multiple Video Device handler Class

    Copyright (c) 2005-2006 by Cláudio da Silveira Pinheiro   <taupter@gmail.com>

    Kopete    (c) 2002-2003      by the Kopete developers  <kopete-devel@kde.org>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU Lesser General Public            *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/

#ifndef KOPETE_AVVIDEODEVICEMODELPOOL_H
#define KOPETE_AVVIDEODEVICEMODELPOOL_H

#include <qstring.h>
#include <qvector.h>
#include <kdebug.h>
#include "kopete_export.h"

namespace Kopete {

namespace AV {

/**
	@author Kopete Developers <kopete-devel@kde.org>
*/
class VideoDeviceModelPool{

	struct VideoDeviceModel
	{
		QString model;
		size_t count;
	};
	QVector<VideoDeviceModel> m_devicemodel;
public:
	VideoDeviceModelPool();
	~VideoDeviceModelPool();
	void clear();
	size_t size();
	size_t addModel(QString newmodel);
};

}

}

#endif
