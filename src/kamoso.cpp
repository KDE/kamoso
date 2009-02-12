/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QListView>
#include <QSplitter>
#include <QPushButton>
#include <QTimer>
#include <QItemDelegate>
#include <QScrollBar>
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
#include "timedpushbutton.h"
#include "countdownwidget.h"

Kamoso::Kamoso(QWidget* parent)
	: KMainWindow(parent)
{
	KConfigGroup general(KGlobal::config(), "General");
	if(general.hasKey("PhotoUrl")) {
		theUrl = general.readEntry("PhotoUrl", KUrl());
	} else {
		KDirSelectDialog dirs;
		
		if(dirs.exec() && dirs.url().isValid()) {
			theUrl = dirs.url();
			general.writeEntry("PhotoUrl", theUrl);
		} else {
			close();
		}
	}
	
	QWidget *innerTopWidget = new QWidget(this);
	QVBoxLayout *layoutTop = new QVBoxLayout(innerTopWidget);
	
	ourView = new ThumbnailView(innerTopWidget);
	o = new KDirOperator(theUrl, 0); //FIXME
	o->setInlinePreviewShown(true);
	o->setIconsZoom(50);
	o->setMimeFilter(QStringList() << "image/png");
	o->setView(ourView);
	ourView->assignDelegate();
	ourView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ourView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ourView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	
	QPushButton *p = new QPushButton(innerTopWidget);
	p->setText(i18n("Take a Picture"));
	p->setIcon(KIcon("webcamreceive"));
	connect(p, SIGNAL(clicked(bool)), SLOT(startCountdown()));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(p);
	buttonsLayout->addStretch();
	
	webcam = new WebcamWidget(innerTopWidget);
	connect(webcam, SIGNAL(photoTaken(KUrl)), SLOT(photoTaken(KUrl)));
	QHBoxLayout *webcamLayout = new QHBoxLayout;
	webcamLayout->addWidget(webcam);
	
	white = new WhiteWidget;
	countdown = new CountdownWidget(this);
	below = new QStackedLayout;
	
	QWidget* viewContainer=new QWidget;
	scrollLeft = new TimedPushButton(KIcon("arrow-left"), QString(), viewContainer, 100);
	scrollRight = new TimedPushButton(KIcon("arrow-right"), QString(), viewContainer, 100);
	connect(scrollLeft, SIGNAL(tick()), SLOT(slotScrollLeft()));
	connect(scrollRight, SIGNAL(tick()), SLOT(slotScrollRight()));
// 	scrollLeft->setEnabled(false);
	
	scrollLeft->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	scrollRight->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	
	QHBoxLayout* viewLayout=new QHBoxLayout(viewContainer);
	viewLayout->setMargin(0);
	viewLayout->setSpacing(0);
	viewLayout->addWidget(scrollLeft);
	viewLayout->addWidget(ourView);
	viewLayout->addWidget(scrollRight);
	
	below->addWidget(viewContainer);
	below->addWidget(countdown);
	
	layoutTop->addLayout(webcamLayout);
	layoutTop->addLayout(buttonsLayout);
	layoutTop->addLayout(below);
	
	setCentralWidget(innerTopWidget);
	connect(countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);
}

Kamoso::~Kamoso()
{
	delete white;
	delete player;
	delete countdown;
	delete o;
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

void Kamoso::photoTaken(const KUrl& url)
{
	o->setCurrentItem(url.path());
// 	qDebug() << "aaaaaaaa" << o->selectedItems() << url;
}

void Kamoso::restore()
{
	white->hide();
	Solid::Control::PowerManager::setBrightness(brightBack);
}


//TODO: Should make it exponential
void Kamoso::slotScrollLeft()
{
	int v=ourView->horizontalScrollBar()->value();
	int min=ourView->horizontalScrollBar()->minimum();
	int max=ourView->horizontalScrollBar()->maximum();
	ourView->horizontalScrollBar()->setValue(qBound(min, v-10, max));
}

void Kamoso::slotScrollRight()
{
	int v=ourView->horizontalScrollBar()->value();
	int min=ourView->horizontalScrollBar()->minimum();
	int max=ourView->horizontalScrollBar()->maximum();
	ourView->horizontalScrollBar()->setValue(qBound(min, v+10, max));
}
