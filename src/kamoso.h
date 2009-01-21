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

class WhiteWidget;
class WebcamWidget;
class CountdownWidget;
class KDirOperator;
class QStackedLayout;
class QSplitter;
class QPushButton;
class ThumbnailView;
namespace Phonon { class MediaObject; }

class Kamoso : public KMainWindow
{
	Q_OBJECT
	public:
		Kamoso ( QWidget *parent=0 );
		~Kamoso();
		
	public slots:
		void takePhoto();
		void startCountdown();
		
	private slots:
		void restore();
		void photoTaken(const KUrl& url);
		void slotScrollLeft();
		void slotScrollRight();
		
	private:
		KUrl theUrl;
		float brightBack;
		
		QStackedLayout *below;
		KDirOperator *o;
		WhiteWidget *white;
		WebcamWidget *webcam;
		CountdownWidget *countdown;
		Phonon::MediaObject *player;
		QPushButton* scrollLeft;
		QPushButton* scrollRight;
		ThumbnailView *ourView;
};

#endif
