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
#include "whitewidgetmanager.h"

const int max_exponential_value = 50;
const int exponential_increment = 5;
Kamoso::Kamoso(QWidget* parent)
	: KMainWindow(parent)
{
	//Check the initial and basic config, and ask for it they don't exist
	this->checkInitConfig();
	
//Small debuggin to know the settings
	qDebug() << "Settings of camoso:";
	qDebug() << "saveUrl: " << Settings::saveUrl();
	qDebug() << "photoTime: " << Settings::photoTime();

//Interface of kamoso, from Top to Bottom
//Layouts are filled when all his childs are created
	QWidget *innerTopWidget = new QWidget(this);//General widget
	QVBoxLayout *layoutTop = new QVBoxLayout(innerTopWidget);//General layuout
	
//First row of the interface
	QHBoxLayout *webcamLayout = new QHBoxLayout;//First Horiz layout, webcam mainly
	webcam = new WebcamWidget(innerTopWidget);//Who paint the webcam stuff
	connect(webcam, SIGNAL(photoTaken(KUrl)), SLOT(photoTaken(KUrl)));
	//Adding webcam to webcamLayout
	webcamLayout->addWidget(webcam);
	
//Second row of the interface
	//Layout that align the buttons (2 row of the app)
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	
	//Button which trhow the startCountDown action (take photo)
	QPushButton *takePictureBtn = new QPushButton(innerTopWidget);
	takePictureBtn->setText(i18n("Take a Picture"));
	takePictureBtn->setIcon(KIcon("webcamreceive"));
	connect(takePictureBtn, SIGNAL(clicked(bool)), SLOT(startCountdown()));

	//Configuration button (Show the configuration dialog)
	QPushButton *configBtn = new QPushButton(innerTopWidget);
	configBtn->setText(i18n("Configure"));
	connect(configBtn,SIGNAL(clicked(bool)),SLOT(configuration()));
	
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(takePictureBtn);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(configBtn);

//Third row of the interface
	stackedBelowLayout = new QStackedLayout;
	QWidget* viewContainer=new QWidget;
	QHBoxLayout* viewLayout=new QHBoxLayout(viewContainer);
	viewLayout->setMargin(0);
	viewLayout->setSpacing(0);
	
	//First column
	scrollLeft = new TimedPushButton(KIcon("arrow-left"), QString(), viewContainer, 100);
	scrollLeft->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	connect(scrollLeft, SIGNAL(tick()), SLOT(slotScrollLeft()));
	connect(scrollLeft, SIGNAL(finished()), SLOT(slotScrollFinish()));
		
	//Second column
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
	
	//Third column
	scrollRight = new TimedPushButton(KIcon("arrow-right"), QString(), viewContainer, 100);
	scrollRight->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	connect(scrollRight, SIGNAL(tick()), SLOT(slotScrollRight()));
	connect(scrollRight, SIGNAL(finished()), SLOT(slotScrollFinish()));
	
	//Filling the view layout(third interface row)
	viewLayout->addWidget(scrollLeft);
	viewLayout->addWidget(customIconView);
	viewLayout->addWidget(scrollRight);
	
	whiteWidgetManager = new WhiteWidgetManager(this);
	countdown = new CountdownWidget(this);
	
	stackedBelowLayout->addWidget(viewContainer);
	stackedBelowLayout->addWidget(countdown);
	
	layoutTop->addLayout(webcamLayout);
	layoutTop->addLayout(buttonsLayout);
	layoutTop->addLayout(stackedBelowLayout);
	
	this->setCentralWidget(innerTopWidget);
	
	connect(countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);
	
	//TODO: find a better place to init this 
	m_exponentialValue = 0;
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
	delete whiteWidgetManager;
	delete player;
	delete countdown;
	delete dirOperator;
	Settings::self()->writeConfig();
}

void Kamoso::startCountdown()
{
	countdown->start();
	stackedBelowLayout->setCurrentIndex(1);
}

void Kamoso::takePhoto()
{
	stackedBelowLayout->setCurrentIndex(0);
	brightBack = Solid::Control::PowerManager::brightness();
	Solid::Control::PowerManager::setBrightness(100);
	whiteWidgetManager->showAll();
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
	whiteWidgetManager->hideAll();
	Solid::Control::PowerManager::setBrightness(brightBack);
}


//TODO: Should make it exponential
void Kamoso::slotScrollLeft()
{
	int v=customIconView->horizontalScrollBar()->value();
	int min=customIconView->horizontalScrollBar()->minimum();
	int max=customIconView->horizontalScrollBar()->maximum();
	customIconView->horizontalScrollBar()->setValue(qBound(min, v-m_exponentialValue, max));
	
	//If this code becomes 1 line larger, export it to a method
	if(m_exponentialValue < max_exponential_value){
		m_exponentialValue += exponential_increment;
	}
}

void Kamoso::slotScrollRight()
{
	int v=customIconView->horizontalScrollBar()->value();
	int min=customIconView->horizontalScrollBar()->minimum();
	int max=customIconView->horizontalScrollBar()->maximum();
	customIconView->horizontalScrollBar()->setValue(qBound(min, v+m_exponentialValue, max));
	
	if(m_exponentialValue < max_exponential_value){
		m_exponentialValue += exponential_increment;
	}
}

void Kamoso::slotScrollFinish()
{
	m_exponentialValue = 0;
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