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
#include <QMenu>
#include <KActionCollection>
#include <KApplication>
#include <KConfigGroup>
#include <KDirOperator>
#include <KDirSelectDialog>
#include <KFileItemDelegate>
#include <KLocale>
#include <Phonon/MediaObject>
#include <KFilePreviewGenerator>
#include <solid/control/powermanager.h>
#include <solid/powermanagement.h>
#include <KStandardDirs>
#include <KConfigDialog>
#include <KDebug>
#include "thumbnailview.h"
#include "whitewidget.h"
#include "webcamwidget.h"
#include "timedpushbutton.h"
#include "countdownwidget.h"
#include "settings.h"
#include "ui_generalConfig.h"
#include "ui_pictureConfig.h"
#include "ui_mainWidget.h"
#include "whitewidgetmanager.h"
#include "webcamretriever.h"
#include "avdevice/videodevicepool.h"
#include <kpluginselector.h>
#include "pluginmanager.h"
#include "kamosoplugin.h"
#include <KMessageBox>

const int max_exponential_value = 50;
const int exponential_increment = 5;
Kamoso::Kamoso(QWidget* parent)
	: KMainWindow(parent)
{
	//Check the initial and basic config, and ask for it they don't exist
	this->checkInitConfig();
	videoRetriever = new WebcamRetriever(NULL,NULL);
	videoRetriever->mVideoDevicePool->scanDevices();
	mVideoDevicePool = Kopete::AV::VideoDevicePool::self();
	connect(videoRetriever,SIGNAL(videoDeviceError()),SLOT(videoDeviceError()));
	connect(videoRetriever->mVideoDevicePool,SIGNAL(deviceRegistered(const QString&)),SLOT(webcamAdded()));
	connect(videoRetriever->mVideoDevicePool,SIGNAL(deviceUnregistered(const QString&)),SLOT(webcamRemoved()));

//Small debuggin to know the settings
	qDebug() << "Settings of camoso:";
	qDebug() << "saveUrl: " << Settings::saveUrl();
	qDebug() << "photoTime: " << Settings::photoTime();

	mainWidgetUi = new Ui::mainWidget;
	mainWidget = new QWidget();
	mainWidgetUi->setupUi(mainWidget);
	
	//We've to investigate if is better call start before do the UI stuff
	if(mVideoDevicePool->size() < 2){
		//At the money there are only 2 widgets to hidden, maybe a container is needed here.
		mainWidgetUi->chooseWebcamLbl->hide();
		mainWidgetUi->webcamCombo->hide();
	}else{
		mainWidgetUi->chooseWebcamLbl->show();
		mainWidgetUi->webcamCombo->show();
		mVideoDevicePool->fillDeviceKComboBox(mainWidgetUi->webcamCombo);
	}
	connect(mainWidgetUi->webcamCombo,SIGNAL(currentIndexChanged(int)),SLOT(webcamChanged(int)));

	videoRetriever->start();
	
//First row Stuff, at the moment only webcam is placed here
	//Setting webcam in the first row, central spot
	webcam = new WebcamWidget(mainWidgetUi->centralSpot,videoRetriever);
	
//Second row Stuff
	//Setting kIcon and conection to the button who take the picture
	mainWidgetUi->takePictureBtn->setIcon(KIcon("webcamreceive"));
 	connect(mainWidgetUi->takePictureBtn, SIGNAL(clicked(bool)), SLOT(startCountdown()));
	
	//Configuration button
	connect(mainWidgetUi->configureBtn, SIGNAL(clicked(bool)), SLOT(configuration()));
	
	//Third row stuff, [btn] <--view-> [btn]
	scrollLeft = new TimedPushButton(KIcon("arrow-left"), QString(),mainWidget, 100);
	scrollLeft->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	connect(scrollLeft, SIGNAL(tick()), SLOT(slotScrollLeft()));
	connect(scrollLeft, SIGNAL(finished()), SLOT(slotScrollFinish()));
	
	//Dir operator will show the previews
	customIconView = new ThumbnailView(mainWidget);//Our custom icon view
	dirOperator = new KDirOperator(saveUrl, this); //FIXME
	dirOperator->setInlinePreviewShown(true);
	dirOperator->setIconsZoom(50);
	dirOperator->setMimeFilter(QStringList() << "image/png");
	dirOperator->setView(customIconView);
	connect(dirOperator, SIGNAL(contextMenuAboutToShow(KFileItem,QMenu*)),
			this, SLOT(contextMenuThumbnails(KFileItem,QMenu*)));
	
	//Tunning a bit the customIconView
	customIconView->assignDelegate();
	customIconView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	customIconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	customIconView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	connect(customIconView, SIGNAL(doubleClicked(QModelIndex)), SLOT(openThumbnail(QModelIndex)));
	
	//Third column
	scrollRight = new TimedPushButton(KIcon("arrow-right"), QString(), mainWidget, 100);
	scrollRight->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	connect(scrollRight, SIGNAL(tick()), SLOT(slotScrollRight()));
	connect(scrollRight, SIGNAL(finished()), SLOT(slotScrollFinish()));
	
	mainWidgetUi->thirdRow->addWidget(scrollLeft);
	mainWidgetUi->thirdRow->addWidget(customIconView);
	mainWidgetUi->thirdRow->addWidget(scrollRight);

	whiteWidgetManager = new WhiteWidgetManager(this);
	countdown = new CountdownWidget(this);
	countdown->hide();
	mainWidgetUi->thirdRow->addWidget(countdown);
	
	connect(countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);
	
	//TODO: find a better place to init this 
	m_exponentialValue = 0;
	this->setCentralWidget(mainWidget);
	
	busyChange(false);
	connect(PluginManager::self(), SIGNAL(busyState(bool)), SLOT(busyChange(bool)));
}

void Kamoso::webcamAdded()
{
	qDebug() << "A new webcam has been added";
	videoRetriever->mLock.lockForWrite();
	qDebug () << "Num of webcams" << videoRetriever->mVideoDevicePool->size();
	qDebug () << "Current Device: " << videoRetriever->mVideoDevicePool->currentDevice();
	if(videoRetriever->mVideoDevicePool->size() < 2){
		videoRetriever->mLock.unlock();
		//At the money there are only 2 widgets to hidden, maybe a container is needed here.
		mainWidgetUi->chooseWebcamLbl->hide();
		mainWidgetUi->webcamCombo->hide();
	}else{
		videoRetriever->mLock.unlock();
		mainWidgetUi->chooseWebcamLbl->show();
		mainWidgetUi->webcamCombo->show();
		videoRetriever->mVideoDevicePool->fillDeviceKComboBox(mainWidgetUi->webcamCombo);
	}
}
void Kamoso::webcamRemoved()
{
	qDebug() << "A new webcam has been removed";
	if(videoRetriever->isRunning() == false){
		restartRetriever();
	}
	videoRetriever->mLock.lockForWrite();
	qDebug () << "Num of webcams" << videoRetriever->mVideoDevicePool->size()-1;
	qDebug () << "Current Device: " << videoRetriever->mVideoDevicePool->currentDevice();
	if((videoRetriever->mVideoDevicePool->size()-1) < 2){
		videoRetriever->mLock.unlock();
		//At the money there are only 2 widgets to hidden, maybe a container is needed here.
		mainWidgetUi->chooseWebcamLbl->hide();
		mainWidgetUi->webcamCombo->hide();
	}else{
		//The combo is already showed (should be),so onlyupdate the content is required.
		videoRetriever->mLock.unlock();
		videoRetriever->mVideoDevicePool->fillDeviceKComboBox(mainWidgetUi->webcamCombo);
	}
}

void Kamoso::retrieverFinished()
{
	qDebug() << "New Thread!";
	delete videoRetriever;
	videoRetriever = new WebcamRetriever(NULL,m_webcamId);
	videoRetriever->start();
	webcam->setRetriever(videoRetriever);
}

void Kamoso::restartRetriever()
{
	qDebug() << "New Thread!2";
	delete videoRetriever;
	videoRetriever = new WebcamRetriever(NULL,NULL);
	videoRetriever->start();
	webcam->setRetriever(videoRetriever);
}

void Kamoso::videoDeviceError()
{
	qDebug() << "Video Device Error";
	if(videoRetriever->isRunning())
	{
		qDebug() << "Thread is currently running. spoting it and restarting";
		videoRetriever->markDone();
		connect(videoRetriever, SIGNAL(finished()), SLOT(restartRetriever()));
	}else{
		qDebug() << "Thread is not longuer running, restarting it";
		restartRetriever();
	}
}

void Kamoso::webcamChanged(const int webcamId)
{
	videoRetriever->mLock.lockForWrite();
	qDebug() << "Current Device: " << videoRetriever->mVideoDevicePool->currentDevice() << "New device: " << webcamId;
	if(videoRetriever->mVideoDevicePool->currentDevice() != webcamId && webcamId > -1){
		videoRetriever->mLock.unlock();
		m_webcamId = webcamId;
		qDebug() << "webcamChanged";
		videoRetriever->markDone();
		connect(videoRetriever, SIGNAL(finished()), SLOT(retrieverFinished()));
	}else{
		videoRetriever->mLock.unlock();
	}
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
	
/**
*This method is called when the configuration button is pushed.
*Show the configuration KCM, and create it if is needed.
*/
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
	//TODO: Check page and pagePicture leaking
	Ui::generalConfigWidget *page = new Ui::generalConfigWidget();
	QWidget *widgetPage = new QWidget();
	page->setupUi(widgetPage);
	page->kcfg_saveUrl->setMode(KFile::Directory);
	Q_EMIT(Settings::saveUrl());
	dialog->addPage(widgetPage,i18n("General"),"ceneral");
	connect(dialog,SIGNAL(settingsChanged(const QString &)), this, SLOT(generalUpdated())); 

	Ui::pictureConfigWidget *pagePicture = new Ui::pictureConfigWidget;
	QWidget *widgetPicturePage = new QWidget();
	pagePicture->setupUi(widgetPicturePage);
	pagePicture->kcfg_photoTime->setValue(Settings::photoTime());
	dialog->addPage(widgetPicturePage,i18n("Photo Settings"),"photoSettings");
	
	//TODO: Use the designer and so on
	KPluginSelector* selector=new KPluginSelector(dialog);
	selector->addPlugins(PluginManager::self()->pluginInfo());
	dialog->addPage(selector, i18n("Plugin List"), "plugins");
	
	dialog->show();
}

/**
*This is called automatically by KCM when the configuration is updated
*/
void Kamoso::generalUpdated()
{
	qDebug() << "Settings New\n" << Settings::saveUrl();
	Settings::self()->writeConfig();
	dirOperator->setUrl(Settings::saveUrl(),false);
}

/**
*Destructor of Kamoso, save configuratoin and delete some stuff
*/
Kamoso::~Kamoso()
{
	delete whiteWidgetManager;
	delete player;
	delete countdown;
	delete dirOperator;
	Settings::self()->writeConfig();
}

/**
*When Take Picture button is pushed, this slot is called
*/
//TODO: Abstraction of what is called on pushBtn?
void Kamoso::startCountdown()
{
	countdown->start();
	//hidding all non-semaphore widgets
	scrollLeft->hide();
	scrollRight->hide();
	customIconView->hide();
	countdown->show();
}

/**
*This Method is called when the countDown is over
*/
void Kamoso::takePhoto()
{
// 	stackedBelowLayout->setCurrentIndex(0);
	scrollLeft->show();
	scrollRight->show();
	customIconView->show();
	countdown->hide();
	qDebug () << "Flash state: " << mainWidgetUi->checkFlash->checkState();
	if(mainWidgetUi->checkFlash->checkState() == 2){
		brightBack = Solid::Control::PowerManager::brightness();
		Solid::Control::PowerManager::setBrightness(100);
		whiteWidgetManager->showAll();
	}
	QTimer::singleShot(1000, this, SLOT(restore()));
	
	KUrl photoPlace = saveUrl;
	photoPlace.addPath(QString("kamoso_%1.png").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")));

	webcam->takePhoto(photoPlace);
	player->play();
}
/**
*This method is called when the picture has been taken
*/
void Kamoso::photoTaken(const KUrl& url)
{
	dirOperator->setCurrentItem(url.path());
}

/**
*This is called after sempahore has end
*/
void Kamoso::restore()
{
	whiteWidgetManager->hideAll();
	Solid::Control::PowerManager::setBrightness(brightBack);
}

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
		bool b=QDesktopServices::openUrl(path);
		if(!b)
			KMessageBox::error(this, "Could not open %1", path.prettyUrl());
	}
}

void Kamoso::contextMenuThumbnails(const KFileItem& item, QMenu* menu)
{
	menu->addSeparator();
	menu->addAction(i18n("Open"), this, SLOT(openThumbnail()));
	
	foreach(KamosoPlugin* p, PluginManager::self()->plugins()) {
		QAction* action=p->thumbnailsAction(item.url());
		if(!action->parent())
			action->setParent(menu);
		
		if(action)
			menu->addAction(action);
	}
}

void Kamoso::busyChange(bool isBusy)
{
	if(isBusy)
		setCaption(i18n("Kamoso - Working..."));
	else
		setCaption(i18n("Kamoso"));
}
