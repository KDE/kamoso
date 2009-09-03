/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@gmail.com>                        *
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
#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include <QWidget>
class KUrl;
class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

class WebcamWidget : public QWidget
{
Q_OBJECT
public:
	WebcamWidget(QWidget* parent);
	~WebcamWidget();
	
public slots:
    void playFile(const QString& file);
	bool takePhoto(const KUrl &dest);
	void recordVideo(const KUrl& dest, bool sound);
signals:
	void photoTaken(const KUrl& dest);
private:
	class Private;
	Private* d;
};
#endif

