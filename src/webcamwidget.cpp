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

/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */
#include "webcamwidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>
#include <KUrl>
#include <KStandardDirs>
#include <KApplication>
#include <KTemporaryFile>
#include <QDebug>
#include <kio/copyjob.h>

Player::Player()
: QWidget()
{
    //preparation of the vlc command
    const char * const vlc_args[] = {
              "-I", "dummy", /* Don't use any interface */
              "--ignore-config", /* Don't use VLC's config */
              "--extraintf=logger", //log anything
              "--verbose=2", //be much more verbose then normal for debugging purpose
              "--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\" };


    _isPlaying=false;

    //Initialize an instance of vlc
    //a structure for the exception is neede for this initalization
    libvlc_exception_init(&_vlcexcep);

    //create a new libvlc instance
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args,&_vlcexcep);  //tricky calculation of the char space used
    raise (&_vlcexcep);
    
    // Create a media player playing environement 
    _mp = libvlc_media_player_new (_vlcinstance, &_vlcexcep);
    raise (&_vlcexcep);

}

//desctructor
Player::~Player()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp, &_vlcexcep);

    /* Free the media_player */
    libvlc_media_player_release (_mp);

    libvlc_release (_vlcinstance);
    raise (&_vlcexcep);
}

void Player::playFile(QString file)
{
    //the file has to be in one of the following formats /perhaps a little bit outdated)
    /*
    [file://]filename              Plain media file
    http://ip:port/file            HTTP URL
    ftp://ip:port/file             FTP URL
    mms://ip:port/file             MMS URL
    screen://                      Screen capture
    [dvd://][device][@raw_device]  DVD device
    [vcd://][device]               VCD device
    [cdda://][device]              Audio CD device
    udp:[[<source address>]@[<bind address>][:<bind port>]]
    */
	
    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new (_vlcinstance, file.toAscii(), &_vlcexcep);
    raise(&_vlcexcep);
    
    libvlc_media_player_set_media (_mp, _m, &_vlcexcep);
    raise(&_vlcexcep);

    // /!\ Please note /!\
    //
    // passing the widget to the lib shows vlc at which position it should show up
    // vlc automatically resizes the video to the ´given size of the widget
    // and it even resizes it, if the size changes at the playing
    
    /* Get our media instance to use our window */
    #if defined(Q_OS_WIN)
        libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(this->winId()), &_vlcexcep );
        //libvlc_media_player_set_hwnd(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
    #elif defined(Q_OS_MAC)
        libvlc_media_player_set_drawable(_mp, this->winId(), &_vlcexcep );
        //libvlc_media_player_set_agl (_mp, _videoWidget->winId(), &_vlcexcep); // for vlc 1.0
    #else //Linux
        libvlc_media_player_set_drawable(_mp, this->winId(), &_vlcexcep );
        //libvlc_media_player_set_xwindow(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
    #endif
    raise(&_vlcexcep);

    /* Play */
    libvlc_media_player_play (_mp, &_vlcexcep );
    raise(&_vlcexcep);

    _isPlaying=true;
}

bool Player::takePhoto(const KUrl &dest)
{
	QString path;
	if(dest.isLocalFile())
	{
		path = dest.toLocalFile();
	}else{
		path=KStandardDirs::locateLocal("appdata","last.png");
	}
	libvlc_video_take_snapshot(_mp, path.toAscii(),640,480, &_vlcexcep);
	if(raise(&_vlcexcep)){
// 		if(dest.isLocalFile())
			emit photoTaken(dest);
// 		else {
// 			KIO::CopyJob* job=KIO::move(KUrl(path), dest);
// 			job->setAutoDelete(true);
// 			connect(job, SIGNAL(copyingDone(KIO::Job*,KUrl,KUrl,time_t,bool,bool)),
// 					this, SLOT(emitKIOPhotoTaken(KIO::Job*,KUrl,KUrl,time_t,bool,bool)));
// 			job->start();
// 		}
	}
	return raise(&_vlcexcep);
}

bool Player::raise(libvlc_exception_t * ex)
{
    if (libvlc_exception_raised (ex))
    {
		qDebug() << "error: %s\n" << libvlc_exception_get_message(ex);
		return false;
    }else{
		return true;
	}
}

