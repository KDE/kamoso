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
#include <kdebug.h>
#include <kconfigdialog.h>
#include "settings.h"
#include "ui_generalConfig.h"
#include "ui_pictureConfig.h"

Kamoso::Kamoso(QWidget* parent)
	: KMainWindow(parent)
{
	//Check the initial and basic config, and ask for it they don't exist
	this->checkInitConfig();
	
//Small debuggin to know the settings
	qDebug() << "Settings of camoso:";
	qDebug() << "saveUrl: " << Settings::saveUrl();
	qDebug() << "photoTime: " << Settings::photoTime();

//Start to create the structure of kamoso (interface level)
	QWidget *innerTopWidget = new QWidget(this);
	QVBoxLayout *layoutTop = new QVBoxLayout(innerTopWidget);
	
//Dir operator will show the previews
	customIconView = new ThumbnailView(innerTopWidget);//Our custom icon view
	dirOperator = new KDirOperator(saveUrl, this); //FIXME
	dirOperator->setInlinePreviewShown(true);
	dirOperator->setIconsZoom(50);
	dirOperator->setMimeFilter(QStringList() << "image/png");
	dirOperator->setView(customIconView);
	
	//Tunning a bit the customIconView
	customIconView->assignDelegate();
	customIconView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	customIconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	customIconView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	connect(customIconView, SIGNAL(doubleClicked(QModelIndex)), SLOT(openThumbnail(QModelIndex)));
	
	//Button which trhow the startCountDown action (take photo)
	QPushButton *takePictureBtn = new QPushButton(innerTopWidget);
	takePictureBtn->setText(i18n("Take a Picture"));
	takePictureBtn->setIcon(KIcon("webcamreceive"));
	connect(takePictureBtn, SIGNAL(clicked(bool)), SLOT(startCountdown()));

	//Configuration button (Show the configuration dialog)
	QPushButton *configBtn = new QPushButton(innerTopWidget);
	configBtn->setText(i18n("Configure"));
	connect(configBtn,SIGNAL(clicked(bool)),SLOT(configuration()));
	
	//Layout that align the buttons
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(takePictureBtn);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(configBtn);
	
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
	viewLayout->addWidget(customIconView);
	viewLayout->addWidget(scrollRight);
	
	below->addWidget(viewContainer);
	below->addWidget(countdown);
	
	layoutTop->addLayout(webcamLayout);
	layoutTop->addLayout(buttonsLayout);
	layoutTop->addLayout(below);
	
	this->setCentralWidget(innerTopWidget);
	connect(countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);
}

void Kamoso::checkInitConfig()
{
	//If kamoso doesn't know where to save the taken photos, ask for it
	if(!Settings::saveUrl().isEmpty()) {
		saveUrl = Settings::saveUrl();
	} else {
		KDirSelectDialog dirs;
		if(dirs.exec() && dirs.url().isValid()) {
			saveUrl = dirs.url();
			Settings::setSaveUrl(saveUrl);
		} else {
			close();
		}
	}
}
	

void Kamoso::configuration()
{
	//If settings dialog is already open, return (and focus)
	if(KConfigDialog::showDialog("settings")){
		return;
	}
//Creating the kcm
	KConfigDialog *dialog = new KConfigDialog(this,"settings",Settings::self());
	dialog->resize(540,dialog->height());
	
	//Widget created with qt-designer
	Ui::generalConfigWidget *page = new Ui::generalConfigWidget;
	QWidget *widgetPage = new QWidget();
	page->setupUi(widgetPage);
	page->kcfg_saveUrl->setMode(KFile::Directory);
	Q_EMIT(Settings::saveUrl);
	dialog->addPage(widgetPage,i18n("General"),"ceneral");
	connect(dialog,SIGNAL(settingsChanged(const QString &)), this, SLOT(generalUpdated())); 

	Ui::pictureConfigWidget *pagePicture = new Ui::pictureConfigWidget;
	QWidget *widgetPicturePage = new QWidget();
	pagePicture->setupUi(widgetPicturePage);
	pagePicture->kcfg_photoTime->setValue(Settings::photoTime());
	dialog->addPage(widgetPicturePage,i18n("Photo Settings"),"photoSettings");
 	dialog->show();
}
void Kamoso::generalUpdated()
{
	qDebug() << "Settings New\n" << Settings::saveUrl();
	Settings::self()->writeConfig();
	dirOperator->setUrl(Settings::saveUrl(),false);
}
Kamoso::~Kamoso()
{
	delete white;
	delete player;
	delete countdown;
	delete dirOperator;
	Settings::self()->writeConfig();
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
	
	KUrl photoPlace = saveUrl;
	photoPlace.addPath(QString("kamoso_%1.png").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")));

	webcam->takePhoto(photoPlace);
	player->play();
}

void Kamoso::photoTaken(const KUrl& url)
{
	dirOperator->setCurrentItem(url.path());
}

void Kamoso::restore()
{
	white->hide();
	Solid::Control::PowerManager::setBrightness(brightBack);
}


//TODO: Should make it exponential
void Kamoso::slotScrollLeft()
{
	int v=customIconView->horizontalScrollBar()->value();
	int min=customIconView->horizontalScrollBar()->minimum();
	int max=customIconView->horizontalScrollBar()->maximum();
	customIconView->horizontalScrollBar()->setValue(qBound(min, v-10, max));
}

void Kamoso::slotScrollRight()
{
	int v=customIconView->horizontalScrollBar()->value();
	int min=customIconView->horizontalScrollBar()->minimum();
	int max=customIconView->horizontalScrollBar()->maximum();
	customIconView->horizontalScrollBar()->setValue(qBound(min, v+10, max));
}

void Kamoso::openThumbnail(const QModelIndex& idx) 
{
	QString filename= idx.data(Qt::DisplayRole).toString();
	if (!filename.isEmpty())
	{
		KUrl path = saveUrl;
		path.addPath(filename);
		QDesktopServices::openUrl(path);
	}
}