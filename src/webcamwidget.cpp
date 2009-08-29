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

#include "webcamwidget.h"

#include <QTimer>
#include <KIcon>
#include <KUrl>
#include <KStandardDirs>
#include <KApplication>
#include <KTemporaryFile>
#include <QDebug>
#include <kio/copyjob.h>
// #include "webcamretriever.h"

WebcamWidget::WebcamWidget(QWidget* parent,WebcamRetriever *videoRetriever) : QLabel(parent)
{
// 	setRetriever(videoRetriever);
// 	qDebug("%d", mRetriever->isAvailable());
}

WebcamWidget::~WebcamWidget()
{
// 	mRetriever->markDone();
}

void WebcamWidget::setRetriever(WebcamRetriever *videoRetriever)
{
// 	this->setMinimumSize(200,200);
// 	setScaledContents(false);
// 	mRetriever = videoRetriever;
// 	connect(mRetriever, SIGNAL(imageReady()), SLOT(slotUpdateImage()));
// 	connect(mRetriever, SIGNAL(finished()), SLOT(mRetreiverFinished()));
// 	connect(mRetriever, SIGNAL(videoDeviceError()), SLOT(mRetreiverFinished()));
}

bool WebcamWidget::takePhoto(const KUrl& dest)
{
// 	if(!mRetriever->isAvailable())
// 		return false;
// 	
// 	QString path;
// 	if(dest.isLocalFile())
// 		path=dest.toLocalFile();
// 	else
// 		path=KStandardDirs::locateLocal("appdata", "last.png");
// 	
// 	mRetriever->lock().lockForRead();
// 	bool ret = mRetriever->image().save(path);
// 	mRetriever->lock().unlock();
// 	
// 	if(ret) {
// 		if(dest.isLocalFile())
// 			emit photoTaken(dest);
// 		else {
// 			KIO::CopyJob* job=KIO::move(KUrl(path), dest);
// 			job->setAutoDelete(true);
// 			connect(job, SIGNAL(copyingDone(KIO::Job*,KUrl,KUrl,time_t,bool,bool)),
// 					this, SLOT(emitKIOPhotoTaken(KIO::Job*,KUrl,KUrl,time_t,bool,bool)));
// 			job->start();
// 		}
// 	}
// 	
// 	return ret;
}

void WebcamWidget::slotUpdateImage()
{
// 	qDebug("pam");
// 	mRetriever->lock().lockForRead();
// 	setPixmap(QPixmap::fromImage(mRetriever->image()));
// 	mRetriever->lock().unlock();
}

void WebcamWidget::mRetreiverFinished()
{
// 	qDebug() << "Restarting webcamWidget";
// 	setAlignment(Qt::AlignCenter);
// 	setPixmap(KIcon("camera-web").pixmap(128,128));
}

QSize WebcamWidget::sizeHint() const
{
// 	return mRetriever->imageSize();
}

void WebcamWidget::emitKIOPhotoTaken(KIO::Job* job, const KUrl& from, const KUrl & to, time_t t, bool , bool )
{
// 	emit photoTaken(to);
}
