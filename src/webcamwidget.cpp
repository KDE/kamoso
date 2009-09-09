/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
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

/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */

#include "webcamwidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>
#include <QDateTime>
#include <KUrl>
#include <KStandardDirs>
#include <KApplication>
#include <KTemporaryFile>
#include <QDebug>
#include <kio/copyjob.h>
#include <vlc/vlc.h>
#include <objectdescriptionmodel.h>
#include <phonon/backendcapabilities.h>

typedef QList<QPair<QByteArray, QString> > PhononDeviceAccessList;
 Q_DECLARE_METATYPE(PhononDeviceAccessList)

struct WebcamWidget::Private
{
	bool raise(libvlc_exception_t * ex);
	bool _isPlaying;
	QByteArray videoTmpPath;
	libvlc_exception_t _vlcexcep;
	libvlc_instance_t *_vlcinstance;
	libvlc_media_player_t *m_mp;
	libvlc_media_t *_m;
};

WebcamWidget::WebcamWidget(QWidget* parent)
	: QWidget(parent), d(new Private)
{
	//preparation of the vlc command
	const char * const vlc_args[] = {
			"-I", "dummy", /* Don't use any interface */
			"--ignore-config", /* Don't use VLC's config */
			"--extraintf=logger", //log anything
			"--verbose=0" //be much more verbose then normal for debugging purpose
			};
	d->_isPlaying=false;

	//Initialize an instance of vlc
	//a structure for the exception is neede for this initalization
	libvlc_exception_init(&d->_vlcexcep);

	//create a new libvlc instance
	d->_vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args,&d->_vlcexcep);  //tricky calculation of the char space used
	d->raise(&d->_vlcexcep);
    
	// Create a media player playing environement 
	d->m_mp = libvlc_media_player_new (d->_vlcinstance, &d->_vlcexcep);
	d->raise(&d->_vlcexcep);
}

//desctructor
WebcamWidget::~WebcamWidget()
{
	/* Stop playing */
	libvlc_media_player_stop (d->m_mp, &d->_vlcexcep);

	/* Free the media_player */
	libvlc_media_player_release (d->m_mp);

	libvlc_release (d->_vlcinstance);
	d->raise(&d->_vlcexcep);
	
	delete d;
}

void WebcamWidget::playFile(const QString& file)
{
	QByteArray mrl("v4l2://");
	mrl.append(file.toAscii());
	mrl.append(":caching=100");
	/* Create a new LibVLC media descriptor */
	d->_m = libvlc_media_new (d->_vlcinstance, mrl, &d->_vlcexcep);
	d->raise(&d->_vlcexcep);

	libvlc_media_player_set_media (d->m_mp, d->_m, &d->_vlcexcep);
	d->raise(&d->_vlcexcep);
    
	/* Get our media instance to use our window */
	#if defined(Q_OS_WIN)
		libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(this->winId()), &_vlcexcep );
		//libvlc_media_player_set_hwnd(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
	#elif defined(Q_OS_MAC)
		libvlc_media_player_set_drawable(_mp, this->winId(), &_vlcexcep );
		//libvlc_media_player_set_agl (_mp, _videoWidget->winId(), &_vlcexcep); // for vlc 1.0
	#else //Linux
		libvlc_media_player_set_drawable(d->m_mp, this->winId(), &d->_vlcexcep );
		//libvlc_media_player_set_xwindow(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
	#endif
	d->raise(&d->_vlcexcep);

	/* Play */
	libvlc_media_player_play (d->m_mp, &d->_vlcexcep );
	d->raise(&d->_vlcexcep);

	d->_isPlaying=true;
}

//TODO: Restore Kio support
bool WebcamWidget::takePhoto(const KUrl &dest)
{
	QString path;
	if(dest.isLocalFile()) {
		path = dest.toLocalFile();
	} else {
		path=KStandardDirs::locateLocal("appdata","last.png");
	}
	
	libvlc_video_take_snapshot(d->m_mp, path.toAscii().data(),640,480, &d->_vlcexcep);
	if(d->raise(&d->_vlcexcep) && !dest.isLocalFile()) {
		KIO::CopyJob* job=KIO::move(KUrl(path), dest);
		job->setAutoDelete(true);
		job->start();
	}
	return d->raise(&d->_vlcexcep);
}

void WebcamWidget::recordVideo(bool sound)
{
	#warning replace /tmp/ by api
	d->videoTmpPath = QString("/tmp/kamoso_%1.ogv").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")).toAscii();
	QByteArray option("sout=#duplicate{dst=display,select=video,dst='transcode{vcodec=theo,vb=1800,scale=1,acodec=vorb,ab=328,channels=2,samplerate=44100}:std{access=file,mux=ogg,dst="+d->videoTmpPath+"}'}");
	if(sound == true){
		QByteArray inputAlsa("input-slave=alsa://");
		inputAlsa.append(phononCaptureDevice());
		libvlc_media_add_option(d->_m,inputAlsa,&d->_vlcexcep);
		libvlc_media_add_option(d->_m,"alsa-caching=100",&d->_vlcexcep);
	}
	
	#warning shouldnt we raise all these exceptions?
	libvlc_media_add_option(d->_m,"sout-display-delay=40",&d->_vlcexcep);
	libvlc_media_add_option(d->_m,"v4l2-standard=0",&d->_vlcexcep);
	libvlc_media_add_option(d->_m,option,&d->_vlcexcep);
	libvlc_media_player_stop(d->m_mp,&d->_vlcexcep);
	d->m_mp = libvlc_media_player_new_from_media(d->_m,&d->_vlcexcep);
	libvlc_media_player_set_drawable(d->m_mp, this->winId(), &d->_vlcexcep );
	libvlc_media_player_play (d->m_mp, &d->_vlcexcep );
}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
	libvlc_media_player_stop(d->m_mp,&d->_vlcexcep);
	if(d->raise(&d->_vlcexcep)) {
		KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
		job->setAutoDelete(true);
		job->start();
	}
}

QString WebcamWidget::phononCaptureDevice()
{
	const QList<Phonon::AudioCaptureDevice> &m_modelData = Phonon::BackendCapabilities::availableAudioCaptureDevices();
	QVariant variantList =  m_modelData.first().property("deviceAccessList");
	PhononDeviceAccessList accessList = variantList.value<PhononDeviceAccessList>();
	QList <QPair <QByteArray, QString > >::const_iterator i, iEnd=accessList.constEnd();
	for(i=accessList.constBegin(); i!=iEnd; ++i) {
		if(i->first == "alsa" && !i->second.contains("phonon")) {
			return i->second;
		}
	}
	
	return QString();
}
bool WebcamWidget::Private::raise(libvlc_exception_t * ex)
{
	if (libvlc_exception_raised (ex))
	{
		qDebug() << "error: %s\n" << libvlc_exception_get_message(ex);
		return false;
	}
	return true;
}
