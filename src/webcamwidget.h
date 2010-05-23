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

#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include "device.h"
#include <QWidget>
#include <KJob>
#include "vlc/libvlc.h"

class KUrl;
class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

namespace KIO { class Job; }

class WebcamWidget : public QWidget
{
Q_OBJECT
public:
	static WebcamWidget* createInstance(QWidget *parent);
	static WebcamWidget* self();
	~WebcamWidget();
	
public slots:
	void playFile(const Device& device);
	bool takePhoto(const KUrl &dest);
	void recordVideo(bool sound);
	void stopRecording(const KUrl& destUrl);
	void fileSaved(KJob *);
	void fileSaved(const KUrl &dest);
	void setBrightness(int level);
	void setSaturation(int level);
	void setContrast(int level);
	void setGamma(int level);
	void setHue(int level);
	void playing();

private:
	WebcamWidget(QWidget* parent);
	static WebcamWidget* s_instance;
	QByteArray phononCaptureDevice();
	void newMedia();
	void setDevice(const Device &device);
	float convertAdjustValue(int level);
	class Private;
	Private* d;
};
#endif

