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

#include "webcamwidget.h"
#include "config-nepomuk.h"
#include "device.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QPainter>

#include <KUrl>
#include <KStandardDirs>
#include <KApplication>
#include <KTemporaryFile>
#include <kio/copyjob.h>
#include <phonon/objectdescriptionmodel.h>
#include <phonon/backendcapabilities.h>
#include <KLocalizedString>
#ifdef HAVE_NEPOMUK
	#include <Nepomuk/ResourceManager>
	#include <Nepomuk/Resource>
	#include <Nepomuk/Tag>
#endif

#ifndef typedef
	#define typeof(X) __typeof__(X)
#endif
#define MODULE_STRING "kamoso"

#include <vlc/vlc.h>
#include <vlc_common.h>
#include <vlc_messages.h>
#include <vlc_vout.h>
#include <vlc_input.h>
#include <vlc/vlc.h>
#include <vlc/libvlc_media_player.h>
#include <cstdlib>

struct libvlc_media_player_t
{
	int i_refcount;
	bool b_own_its_input_thread;
	vlc_mutex_t object_lock;
	input_thread_t *p_input_thread;
	libvlc_media_t *p_md;
	libvlc_event_manager_t *p_event_manager;
	struct
	{
		void *hwnd;
		void *nsobject;
		uint32_t xid;
		uint32_t agl;
	} drawable;
	struct libvlc_instance_t *p_libvlc_instance;
};

typedef void (*libvlc_vlm_release_func_t)( libvlc_instance_t * ) ;
typedef struct libvlc_vlm_t
{
    vlm_t                  *p_vlm;
    libvlc_event_manager_t *p_event_manager;
    libvlc_vlm_release_func_t pf_release;
} libvlc_vlm_t;

struct libvlc_instance_t
{
    libvlc_int_t *p_libvlc_int;
    libvlc_vlm_t  libvlc_vlm;
    unsigned      ref_count;
    int           verbosity;
    vlc_mutex_t   instance_lock;
    struct libvlc_callback_entry_list_t *p_callback_list;
};

typedef QList<QPair<QByteArray, QString> > PhononDeviceAccessList;
 Q_DECLARE_METATYPE(PhononDeviceAccessList)

struct WebcamWidget::Private
{
	QByteArray videoTmpPath;
	QString playingFile;
	QStringList effects;
	Device device;
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *player;
	libvlc_media_t *media;
	libvlc_event_manager_t *eventManager;
	vlc_object_t *vlcMainObject;
	int brightness;
};

void callback( const libvlc_event_t *ev, void *param )
{
// 	if(ev->type == libvlc_MediaPlayerPositionChanged) {
// 		WebcamWidget::self()->playing();
// 	}
}

WebcamWidget *WebcamWidget::s_instance = NULL;

WebcamWidget* WebcamWidget::createInstance(QWidget *parent)
{
	if(s_instance == NULL) {
		s_instance = new WebcamWidget(parent);
	}
	return s_instance;
}

WebcamWidget* WebcamWidget::self()
{
	//TODO: add error reporting if WebcamWidget is not instance
	return s_instance;
}

WebcamWidget::WebcamWidget(QWidget* parent)
	: QWidget(parent), d(new Private)
{

	//widget preparation
	// When resizing fill with black (backgroundRole color) the rest is done by paintEvent
	setAttribute(Qt::WA_OpaquePaintEvent);

	// Disable Qt composition management as MPlayer draws onto the widget directly
	setAttribute(Qt::WA_PaintOnScreen);

	// Indicates that the widget has no background,
	// i.e. when the widget receives paint events, the background is not automatically repainted.
	setAttribute(Qt::WA_NoSystemBackground);

	//preparation of the vlc command
	const char * const vlc_args[] = {
			"--intf=dummy",
			"--ignore-config",
			"--reset-plugins-cache",
			"--no-media-library",
			"--no-one-instance",
			"--no-osd",
			"--no-stats",
			"--no-video-title-show",
			"--album-art=0"
			};

	//create a new libvlc instance
	d->vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if(!d->vlcInstance)
		qDebug() << "libvlc exception:" << libvlc_errmsg();

	//Try to get the main object
// 	vlc_object_hold(d->vlcInstance->p_libvlc_int);
// 	d->vlcMainObject = (vlc_object_t*) d->vlcInstance->p_libvlc_int;

	// Create a media player playing environement 
	d->player = libvlc_media_player_new (d->vlcInstance);
	if(!d->player)
		qDebug() << "libvlc exception:" << libvlc_errmsg();

	libvlc_video_set_adjust_int(d->player, libvlc_adjust_Enable, 1);

	d->effects.append("wave");
}

void WebcamWidget::paintEvent(QPaintEvent *p_event)
{
    // FIXME this makes the video flicker
    // Make everything backgroundRole color
    //afiestas: ATM our vout is not resizable, so we dont't care
    QPainter painter(this);
    painter.eraseRect(rect());
}

//desctructor
WebcamWidget::~WebcamWidget()
{
	libvlc_media_release(d->media);

	#warning We have to implement an asynchronous shut down for vlc!
	libvlc_media_player_stop (d->player);

	libvlc_media_player_release (d->player);

	libvlc_release (d->vlcInstance);

// 	delete d;
}

void WebcamWidget::playing()
{
// 	libvlc_event_detach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL);
}

void WebcamWidget::playFile(const Device &device)
{
	qDebug() << "playFile called" << device.path();;
	setDevice(device);

	//Create a new media and set it in d->media pointer
	newMedia();

	libvlc_media_player_set_media (d->player, d->media);

	libvlc_media_player_set_xwindow(d->player, this->winId() );
// 	libvlc_event_attach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL);

	/* Play */
	if (!libvlc_media_player_play (d->player))
		qDebug() << "libvlc exception:" << libvlc_errmsg();
}

void WebcamWidget::setDevice(const Device &device) {
	d->device = device;
	d->playingFile = device.path();
	d->brightness = device.brightness();
}
bool WebcamWidget::takePhoto(const KUrl &dest)
{
	playing();
	QString path;
	if(dest.isLocalFile()) {
		path = dest.toLocalFile();
	} else {
		path=KStandardDirs::locateLocal("appdata","last.png");
	}

	int result = libvlc_video_take_snapshot(d->player, 0, path.toAscii().data(),this->width(),this->height());
	if (!result)
		qDebug() << "Take snapshot error, video not found";

	if(!dest.isLocalFile()) {
		KIO::CopyJob* job=KIO::move(KUrl(path), dest);
		connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
		job->setAutoDelete(true);
		job->start();
	} else {
		fileSaved(dest);
	}
	return true;
}

void WebcamWidget::fileSaved(const KUrl &dest) {
	#ifdef HAVE_NEPOMUK
		if(Nepomuk::ResourceManager::instance()->initialized()) {
			qDebug() << dest;
			Nepomuk::Tag tag("kamoso");
			Nepomuk::Resource file(QUrl(dest.toLocalFile()));
			file.addTag(tag);
			file.addTag(tag);//Maybe is my computer, but I need to do ths twice, I'll it investigate later
		}
	#endif
}

void WebcamWidget::fileSaved(KJob *job)
{
	KIO::CopyJob *copy = static_cast<KIO::CopyJob *>(job);
	fileSaved(copy->destUrl());
}

void WebcamWidget::recordVideo(bool sound)
{
	libvlc_media_player_stop(d->player);
	libvlc_media_player_release(d->player);
	d->videoTmpPath = QString(QDir::tempPath() + "/kamoso_%1.avi").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")).toAscii();
	QByteArray option("sout=#duplicate{dst=display,select=video,dst='transcode{vcodec=xvid,vb=1800,ab=352,acodec=vorb,samplerate=44100,fps=25}:std{access=file,mux=avi,dst="+d->videoTmpPath+"}'}");

	if(sound == true){
		QByteArray inputAlsa("input-slave=alsa://");
		inputAlsa.append(phononCaptureDevice());

		libvlc_media_add_option(d->media,inputAlsa);

		libvlc_media_add_option(d->media,"alsa-caching=100");

		libvlc_media_add_option(d->media,"alsa-samplerate=44100");
	}

	libvlc_media_add_option(d->media,"sout-display-delay=40");

	libvlc_media_add_option(d->media,"v4l2-standard=0");

	libvlc_media_add_option(d->media,option);

	d->player = libvlc_media_player_new_from_media(d->media);
	libvlc_video_set_adjust_int(d->player, libvlc_adjust_Enable, 1);
	if(!d->player)
		qDebug() << "libvlc exception:" << libvlc_errmsg();

	libvlc_media_player_set_xwindow(d->player, this->winId() );

// 	libvlc_event_attach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL);
	if (!libvlc_media_player_play (d->player ))
		qDebug() << "libvlc exception:" << libvlc_errmsg();
}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
	libvlc_media_player_stop(d->player);
	libvlc_media_release(d->media);

	KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
	connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
	job->setAutoDelete(true);
	job->start();
}

QByteArray WebcamWidget::phononCaptureDevice()
{
	const QList<Phonon::AudioCaptureDevice> &m_modelData = Phonon::BackendCapabilities::availableAudioCaptureDevices();
	QVariant variantList =  m_modelData.first().property("deviceAccessList");
	PhononDeviceAccessList accessList = variantList.value<PhononDeviceAccessList>();
	QList <QPair <QByteArray, QString > >::const_iterator i, iEnd=accessList.constEnd();
	for(i=accessList.constBegin(); i!=iEnd; ++i) {
		if(i->first == "alsa" && !i->second.contains("phonon")) {
			return i->second.toAscii();
		}
	}
	
	return QByteArray();
}

void WebcamWidget::setBrightness(int level)
{
	libvlc_video_set_adjust_float(d->player, libvlc_adjust_Brightness, convertAdjustValue(level));
}
void WebcamWidget::setContrast(int level)
{
	libvlc_video_set_adjust_float(d->player, libvlc_adjust_Contrast, convertAdjustValue(level));
}
void WebcamWidget::setSaturation(int level)
{
	libvlc_video_set_adjust_float(d->player, libvlc_adjust_Saturation, convertAdjustValue(level));
}
void WebcamWidget::setGamma(int level)
{
	libvlc_video_set_adjust_float(d->player, libvlc_adjust_Gamma, convertAdjustValue(level));
}
void WebcamWidget::setHue(int level)
{
	libvlc_video_set_adjust_int(d->player, libvlc_adjust_Hue, level);
}
float WebcamWidget::convertAdjustValue(int level){
	return (float)level / 100.0f;
}
void WebcamWidget::newMedia()
{
	QByteArray mrl("v4l2://");
	mrl.append(d->playingFile);
	mrl.append(":caching=100 :v4l2-controls-reset");

	d->media = libvlc_media_new_location (d->vlcInstance, mrl);
	if(!d->media)
		qDebug() << "libvlc exception:" << libvlc_errmsg();

	if (!d->effects.isEmpty()) {
		QString effectString;
		foreach(const QString effect,d->effects) {
			effectString.append(effect+":");
		}

		qDebug() << "Adding effect string: " << effectString;
		libvlc_media_add_option(d->media,":video-filter="+effectString.toAscii());
		libvlc_media_add_option(d->media,":sout-transcode-vfilter="+effectString.toAscii());
	}

}