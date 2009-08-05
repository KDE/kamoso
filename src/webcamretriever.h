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

#ifndef WEBCAMRETRIEVER_H
#define WEBCAMRETRIEVER_H

#include <QThread>
#include <QPixmap>
#include <QReadWriteLock>

class QTimer;
namespace Kopete { namespace AV { class VideoDevicePool; } }

class WebcamRetriever : public QThread
{
	Q_OBJECT
	public:
		WebcamRetriever(QObject* parent=0, int webcamId=NULL);
		
		virtual void run();
		bool isAvailable() const;
		void markDone() { mDone=true; }
		
		QReadWriteLock & lock() { return mLock; }
		const QImage& image() const { return mImage; }
		QSize imageSize() const { return mImageSize; }

		Kopete::AV::VideoDevicePool *mVideoDevicePool;
	signals:
		void imageReady();
		void initialized();
	
	private:
		int m_webcamId;
		bool mInitialized;
		bool mDone;
		QImage mImage;
		QSize mImageSize;
		
		QReadWriteLock mLock;
};

#endif // WEBCAMRETRIEVER_H
