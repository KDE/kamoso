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

typedef QList<QPair<QByteArray, QString> > PhononDeviceAccessList;
 Q_DECLARE_METATYPE(PhononDeviceAccessList)

struct WebcamWidget::Private
{
	bool raise(libvlc_exception_t * ex);
	QByteArray videoTmpPath;
	QString playingFile;
	QStringList effects;
	Device device;
	libvlc_exception_t vlcException;
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *player;
	libvlc_media_t *media;
	libvlc_event_manager_t *eventManager;
	vlc_object_t *vlcMainObject;
	vlc_object_t *videoOutput;
	int brightness;
};

void callback( const libvlc_event_t *ev, void *param )
{
	if(ev->type == libvlc_MediaPlayerPositionChanged) {
		WebcamWidget::self()->playing();
	}
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
	//preparation of the vlc command
	const char * const vlc_args[] = {
			"-I", "dummy", /* Don't use any interface */
			"--ignore-config", /* Don't use VLC's config */
			"--extraintf=logger", //log anything
			"--verbose=0" //be much more verbose then normal for debugging purpose
			};

	//Initialize an instance of vlc
	//a structure for the exception is neede for this initalization
	libvlc_exception_init(&d->vlcException);

	//create a new libvlc instance
	d->vlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args,&d->vlcException);  //tricky calculation of the char space used
	d->raise(&d->vlcException);

	d->vlcMainObject = libvlc_get_vlc_instance(d->vlcInstance);

	// Create a media player playing environement 
	d->player = libvlc_media_player_new (d->vlcInstance, &d->vlcException);
	d->eventManager = libvlc_media_player_event_manager(d->player,&d->vlcException);
	d->raise(&d->vlcException);
	
// 	d->effects.append("adjust");
}

//desctructor
WebcamWidget::~WebcamWidget()
{
	libvlc_media_player_stop (d->player, &d->vlcException);

	libvlc_media_player_release (d->player);

	libvlc_release (d->vlcInstance);
	
	delete d;
}

int counter = 0;
void WebcamWidget::retro(vlc_object_t* object)
{
	counter ++;
	QString tabs("");
	for(int x=0;x < counter;x++) {
		tabs.append("    ");
	}
	vlc_list_t* list =  vlc_list_children(object);
	for( int i = 0; i < list->i_count ; i++ )
	{
// 		qDebug() << "=" << list->p_values[i].p_object->psz_object_type << "=";
		if(strcmp("video output",list->p_values[i].p_object->psz_object_type) == 0) {
			qDebug() << tabs << "Insideeeeeeeeeeeeeeee";
			d->videoOutput = list->p_values[i].p_object;
// 			var_SetString(list->p_values[i].p_object,"video-filter","wave:invert");
			sleep(1);
		}
		if(strcmp("filter",list->p_values[i].p_object->psz_object_type) == 0) {
			qDebug() << list->p_values[i].p_object->psz_object_type;
		}
		qDebug() << tabs << list->p_values[i].p_object->psz_object_type;
		retro(list->p_values[i].p_object);
		counter --;
	}
}

void WebcamWidget::playing()
{
	libvlc_event_detach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL,&d->vlcException);
	retro(d->vlcMainObject);
}
void WebcamWidget::playFile(const Device &device)
{
	qDebug() << "playFile called" << device.path();;
	setDevice(device);

	//Create a new media and set it in d->media pointer
	newMedia();

	libvlc_media_player_set_media (d->player, d->media, &d->vlcException);
	d->raise(&d->vlcException);

	libvlc_media_player_set_xwindow(d->player, this->winId(), &d->vlcException );
	d->raise(&d->vlcException);
	libvlc_event_attach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL,&d->vlcException);

	/* Play */
	libvlc_media_player_play (d->player, &d->vlcException );
	d->raise(&d->vlcException);
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

	libvlc_video_take_snapshot(d->player, path.toAscii().data(),this->width(),this->height(), &d->vlcException);
	d->raise(&d->vlcException);
	if(d->raise(&d->vlcException) && !dest.isLocalFile()) {
		KIO::CopyJob* job=KIO::move(KUrl(path), dest);
		connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
		job->setAutoDelete(true);
		job->start();
	} else {
		fileSaved(dest);
	}
	return d->raise(&d->vlcException);
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
	d->videoTmpPath = QString(QDir::tempPath() + "/kamoso_%1.ogv").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")).toAscii();
	QByteArray option("sout=#duplicate{dst=display,select=video,dst='transcode{vcodec=theo,vb=1800,scale=1,acodec=vorb,ab=328,channels=2,samplerate=44100}:std{access=file,mux=ogg,dst="+d->videoTmpPath+"}'}");

	if(sound == true){
		QByteArray inputAlsa("input-slave=alsa://");
		inputAlsa.append(phononCaptureDevice());

		libvlc_media_add_option(d->media,inputAlsa,&d->vlcException);
		d->raise(&d->vlcException);

		libvlc_media_add_option(d->media,"alsa-caching=100",&d->vlcException);
		d->raise(&d->vlcException);
	}

	libvlc_media_add_option(d->media,"sout-display-delay=40",&d->vlcException);
	d->raise(&d->vlcException);

	libvlc_media_add_option(d->media,"v4l2-standard=0",&d->vlcException);
	d->raise(&d->vlcException);

	libvlc_media_add_option(d->media,option,&d->vlcException);
	d->raise(&d->vlcException);

	libvlc_media_player_stop(d->player,&d->vlcException);
	d->raise(&d->vlcException);

	d->player = libvlc_media_player_new_from_media(d->media,&d->vlcException);
	d->raise(&d->vlcException);

	libvlc_media_player_set_xwindow(d->player, this->winId(), &d->vlcException );
	d->raise(&d->vlcException);

	libvlc_event_attach(d->eventManager,libvlc_MediaPlayerPositionChanged,callback,NULL,&d->vlcException);
	
	libvlc_media_player_play (d->player, &d->vlcException );
	d->raise(&d->vlcException);
}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
	libvlc_media_player_stop(d->player,&d->vlcException);
	libvlc_media_release(d->media);

	if(d->raise(&d->vlcException)) {
		KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
		connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
		job->setAutoDelete(true);
		job->start();
	}
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
	vlc_object_t *found = (vlc_object_t*) vlc_object_find_name(d->videoOutput,"adjust",FIND_CHILD);
	var_SetFloat(found,"brightness",convertAdjustValue(level));
}
void WebcamWidget::setContrast(int level)
{
	vlc_object_t *found = (vlc_object_t*) vlc_object_find_name(d->videoOutput,"adjust",FIND_CHILD);
	var_SetFloat(found,"contrast",convertAdjustValue(level));
}
void WebcamWidget::setSaturation(int level)
{
	vlc_object_t *found = (vlc_object_t*) vlc_object_find_name(d->videoOutput,"adjust",FIND_CHILD);
	var_SetFloat(found,"saturation",convertAdjustValue(level));
}
void WebcamWidget::setGamma(int level)
{
	vlc_object_t *found = (vlc_object_t*) vlc_object_find_name(d->videoOutput,"adjust",FIND_CHILD);
	var_SetFloat(found,"gamma",convertAdjustValue(level));
}
void WebcamWidget::setHue(int level)
{
	vlc_object_t *found = (vlc_object_t*) vlc_object_find_name(d->videoOutput,"adjust",FIND_CHILD);
	var_SetFloat(found,"hue",level);
}
float WebcamWidget::convertAdjustValue(int level){
	return (float)level / 100.0f;
}
void WebcamWidget::newMedia()
{
	QByteArray mrl("v4l2://");
	mrl.append(d->playingFile);
	mrl.append(":caching=100 :no-video-title-show :v4l2-controls-reset");

	d->media = libvlc_media_new (d->vlcInstance, mrl, &d->vlcException);
	d->raise(&d->vlcException);

	QString effectString;
	foreach(QString effect,d->effects) {
		effectString.append(effect+":");
	}
	
	//The adjust effect is an special case, since we need it preconfigured
	//Maybe we'll need a map of maps to save effects properties
	effectString.append("adjust{");
	effectString.append("brightness="+QString::number(convertAdjustValue(d->device.brightness()))+",");
	effectString.append("contrast="+QString::number(convertAdjustValue(d->device.contrast()))+",");
	effectString.append("saturation="+QString::number(convertAdjustValue(d->device.saturation()))+",");
	effectString.append("gamma="+QString::number(convertAdjustValue(d->device.gamma()))+",");
	effectString.append("hue="+QString::number(d->device.hue()));
	effectString.append("}");
	qDebug() << effectString;
	libvlc_media_add_option(d->media,"video-filter="+effectString.toAscii(),&d->vlcException);
	libvlc_media_add_option(d->media,"sout-transcode-vfilter="+effectString.toAscii(),&d->vlcException);
	d->raise(&d->vlcException);
}

bool WebcamWidget::Private::raise(libvlc_exception_t * ex)
{
	if (libvlc_exception_raised (ex))
	{
		qDebug() << "VLC error %s\n" << libvlc_exception_get_message(ex);
		return false;
	}
	return true;
}