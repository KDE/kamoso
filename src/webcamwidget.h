/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */
#ifndef VLC_ON_QT_H
#define VLC_ON_QT_H

#include <vlc/vlc.h>

#include <QWidget>
#include <KUrl>
class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

class WebcamWidget : public QWidget
{
Q_OBJECT
public:
	WebcamWidget();
	~WebcamWidget();
	
private:
	QString m_filePath;
public slots:
    void playFile(const QString& file);
	bool takePhoto(const KUrl &dest);
	void recordVideo(const KUrl& dest, bool sound);
signals:
	void photoTaken(const KUrl& dest);
private:
	bool raise(libvlc_exception_t * ex);
	bool _isPlaying;
	libvlc_exception_t _vlcexcep;
	libvlc_instance_t *_vlcinstance;
	libvlc_media_player_t *m_mp;
	libvlc_media_t *_m;
};
#endif

