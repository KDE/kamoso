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

#ifndef KAMOSO_H
#define KAMOSO_H

#include <KMainWindow>
#include <KUrl>
#include <ui_mainWidget.h>

class QRadioButton;
class ShootMode;
class WhiteWidgetManager;
class WebcamWidget;
class WebcamRetriever;
class CountdownWidget;
class KDirOperator;
class QStackedLayout;
class QSplitter;
class QPushButton;
class QModelIndex;
class ThumbnailView;
class KFileItem;
class WebcamWidget;
class DeviceManager;
class KamosoJob;

namespace Phonon { class MediaObject; }
class Kamoso : public KMainWindow
{
	Q_OBJECT
//Methods that aren't slots
	public:
		Kamoso ( QWidget *parent=0 );
		void checkInitConfig();
		~Kamoso();
		
//Only slots
	public slots:
		void takePhoto();
		void startVideo(bool recording);
		void startCountdown();
		void configuration();
		void generalUpdated();
		void webcamChanged(int index);
		void webcamAdded();
		void webcamRemoved();
		void contextMenuThumbnails(const KFileItem& item, QMenu* menu);
		void thumbnailAdded();
		void selectLast();
		void selectJob(KamosoJob*);
		void changeMode(bool);
		
	private slots:
		void restore();
		void slotScrollLeft();
		void slotScrollRight();
		void slotScrollFinish();
		void openThumbnail(const QModelIndex& idx);
		void openThumbnail(const QList<KUrl>& url);
		void fillKcomboDevice();
	signals:
		void webcamPlaying(const QString&);
		
	private:
		KUrl saveUrl;
		float brightBack;

		WebcamRetriever *videoRetriever;
		int m_webcamId;
		KDirOperator *dirOperator;
		WhiteWidgetManager *whiteWidgetManager;
		WebcamWidget *webcam;
		CountdownWidget *countdown;
		Phonon::MediaObject *player;
		QPushButton* scrollLeft;
		QPushButton* scrollRight;
		ThumbnailView *customIconView;
		Ui::mainWidget *mainWidgetUi;
		QWidget *mainWidget;
		DeviceManager *deviceManager;
		int m_exponentialValue;
		QList<ShootMode*> m_modes;
		QList<QRadioButton*> m_modesRadio;
};

#endif
