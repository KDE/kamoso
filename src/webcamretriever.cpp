/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@gmail.com>                             *
 *  Copyright (C) 2008 by Alex Fiestas <alex@eyeos.org>                              *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 3                   *
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

#include "webcamretriever.h"
#include "avdevice/videodevicepool.h"
#include <QTimer>

const int refresh=10;

WebcamRetriever::WebcamRetriever(QObject* parent)
	: QThread(parent), mInitialized(false), mDone(false), mImageSize(640, 480)
{}

void WebcamRetriever::run()
{
	mVideoDevicePool = Kopete::AV::VideoDevicePool::self();
	mVideoDevicePool->open();
	mVideoDevicePool->setSize(mImageSize.width(), mImageSize.height());
	if(!mVideoDevicePool->hasDevices())
	return;
	mVideoDevicePool->startCapturing();
	mInitialized=true;
	
	emit initialized();
	for(;!mDone;) {
		mLock.lockForWrite();
		mVideoDevicePool->getFrame();
		mVideoDevicePool->getImage(&mImage);
		mLock.unlock();
		
// 		qDebug() << "pim";
		emit imageReady();
		msleep(refresh);
	}
}

bool WebcamRetriever::isAvailable() const
{
	return mInitialized && mVideoDevicePool->hasDevices();
}
