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

#include "kamoso.h"
#include <QLayout>
#include <QStackedLayout>
#include <QListView>
#include <QSplitter>
#include <QPushButton>
#include <QTimer>
#include <QItemDelegate>
#include <KActionCollection>
#include <KApplication>
#include <KConfigGroup>
#include <KDirOperator>
#include <KDirSelectDialog>
#include <KFileItemDelegate>
#include <KLocale>
#include <Phonon/MediaObject>
#include <kfilepreviewgenerator.h>
#include <solid/control/powermanager.h>
#include <solid/powermanagement.h>
#include <kstandarddirs.h>
#include "thumbnailview.h"
#include "whitewidget.h"
#include "webcamwidget.h"
#include "countdownwidget.h"
#include "customDelegate.h"

Kamoso::Kamoso(QWidget* parent)
  : KMainWindow(parent)
  , splitter(new QSplitter(this))
{
	KConfigGroup general(KGlobal::config(), "General");
	if(general.hasKey("PhotoUrl")) {
		theUrl = general.readEntry("PhotoUrl", KUrl());
	} else {
		KDirSelectDialog dirs;
		
		if(dirs.exec()) {
			theUrl = dirs.url();
			general.writeEntry("PhotoUrl", theUrl);
		} else {
			close();
		}
	}
	
	
	QWidget *innerTopWidget = new QWidget(this);
	QVBoxLayout *layoutTop = new QVBoxLayout(innerTopWidget);

	QWidget *innerBottomWidget = new QWidget(this);
	QVBoxLayout *layoutBottom = new QVBoxLayout(innerBottomWidget);
	
	QListView *ourView = new ThumbnailView(innerTopWidget);
	o = new KDirOperator(theUrl, this);

	o->setInlinePreviewShown(true);
	o->setIconsZoom(50);
	o->setMimeFilter(QStringList() << "image/png");
	o->setView(ourView);
	ourView->setItemDelegate(new CustomDelegate(ourView));
	
	QPushButton *p = new QPushButton(innerTopWidget);
	p->setText(i18n("Take a Picture"));
	p->setIcon(KIcon("webcamreceive"));
	connect(p, SIGNAL(clicked(bool)), SLOT(startCountdown()));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(p);
	buttonsLayout->addStretch();
	
	webcam = new WebcamWidget(innerTopWidget);
	QHBoxLayout *webcamLayout = new QHBoxLayout;
	webcamLayout->addWidget(webcam);
	
	white = new WhiteWidget;
	countdown = new CountdownWidget(this);
	below = new QStackedLayout;
	
	layoutTop->addLayout(webcamLayout);
	layoutTop->addLayout(buttonsLayout);
	layoutBottom->addLayout(below);
	
	below->addWidget(o);
	below->addWidget(countdown);
	
	splitter->addWidget(innerTopWidget);
	splitter->addWidget(innerBottomWidget);

	setCentralWidget(splitter);
	connect(countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);

	splitter->restoreState(general.readEntry("splitterState", QByteArray()));
}

Kamoso::~Kamoso()
{
  KConfigGroup cg(KGlobal::config(), "General");
  cg.writeEntry("splitterState", splitter->saveState());

  delete white;
  delete player;
  delete countdown;
}

void Kamoso::startCountdown()
{
	countdown->start();
	below->setCurrentIndex(1);
}

void Kamoso::takePhoto()
{
	below->setCurrentIndex(0);
	brightBack = Solid::Control::PowerManager::brightness();
	Solid::Control::PowerManager::setBrightness(100);
	white->showFullScreen();
	QTimer::singleShot(1000, this, SLOT(restore()));
	
	KUrl photoPlace = theUrl;
	photoPlace.addPath(QString("kamoso_%1.png").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")));

	webcam->takePhoto(photoPlace);
	player->play();
}

void Kamoso::restore()
{
	white->hide();
	Solid::Control::PowerManager::setBrightness(brightBack);
}
