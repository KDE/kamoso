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

#ifndef KAMOSO_H
#define KAMOSO_H

#include <KMainWindow>
#include <KUrl>
#include <ui_mainWidget.h>

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
namespace Phonon { class MediaObject; }

class Kamoso : public KMainWindow
{
	Q_OBJECT
//Methods that aren't slots
	public:
		Kamoso ( QWidget *parent=0 );
		void checkInitConfig();
		~Kamoso();
		WebcamRetriever *videoRetriever;
		int m_webcamId;
	private:
		KUrl saveUrl;
		float brightBack;

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
		int m_exponentialValue;
//Only slots
	public slots:
		void takePhoto();
		void startCountdown();
		void configuration();
		void generalUpdated();
		void webcamChanged(const int webcamId);
		void webcamAdded(const QString & udi );
		void webcamRemoved(const QString & udi );
	private slots:
		void restore();
		void photoTaken(const KUrl& url);
		void slotScrollLeft();
		void slotScrollRight();
		void slotScrollFinish();
		void retrieverFinished();
		void openThumbnail(const QModelIndex& idx);
};

#endif
