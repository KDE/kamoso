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

#include "kamoso.h"
#include "config-nepomuk.h"
#ifdef HAVE_NEPOMUK
	#include <Nepomuk/ResourceManager>
	#include <Nepomuk/Resource>
	#include <Nepomuk/Tag>
#endif
#include <QLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QMenu>
#include <KActionCollection>
#include <KApplication>
#include <KConfigGroup>
#include <KDirOperator>
#include <KDirSelectDialog>
#include <KLocale>
#include <KStandardDirs>
#include <KConfigDialog>
#include <KDebug>
#include <KPluginSelector>
#include <KMessageBox>
#include <KStatusBar>
#include <Phonon/MediaObject>
#include <solid/control/powermanager.h>
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
#include "devicemanager.h"
#include "pluginmanager.h"
#include "kamosoplugin.h"
#include "kamosojobtracker.h"
#include "kamosojob.h"
#include "photoshootmode.h"
#include "videoshootmode.h"
#include "burstshootmode.h"

const int max_exponential_value = 50;
const int exponential_increment = 5;
Kamoso::Kamoso(QWidget* parent)
	: KMainWindow(parent),dirOperator(0), m_activeMode(0), m_flashEnabled(true)
{
	m_countdown = new CountdownWidget(this);
	m_countdown->hide();
	
	//Check the initial and basic config, and ask for it they don't exist
	this->checkInitConfig();

	deviceManager = DeviceManager::self();
	connect(deviceManager,SIGNAL(deviceRegistered(QString)),SLOT(webcamAdded()));
	connect(deviceManager,SIGNAL(deviceUnregistered(QString)),SLOT(webcamRemoved()));
	
	qDebug() << "Settings of kamoso:";
	qDebug() << "saveUrl: " << Settings::saveUrl();
	qDebug() << "photoTime: " << Settings::photoTime();

	mainWidgetUi = new Ui::mainWidget;
	mainWidget = new QWidget(this);
	mainWidgetUi->setupUi(mainWidget);
	
	//We've to investigate if is better call start before do the UI stuff
	bool comboShown=deviceManager->numberOfDevices() > 1;
	mainWidgetUi->chooseWebcamLbl->setVisible(comboShown);
	mainWidgetUi->webcamCombo->setVisible(comboShown);
	
	connect(this,SIGNAL(webcamPlaying(const QString&)),deviceManager,SLOT(webcamPlaying(const QString&)));
//First row Stuff, at the moment only webcam is placed here
	//Setting webcam in the first row, central spot
	
	webcam = new WebcamWidget(this);
	webcam->setParent(mainWidgetUi->centralSpot);
	webcam->setMinimumSize(640,480);
	if(deviceManager->hasDevices()) {
		webcam->playFile(deviceManager->defaultDevicePath());
		emit webcamPlaying(deviceManager->defaultDeviceUdi());
	} //TODO: else we should warn the user
// 	connect(webcam, SIGNAL(photoTaken(KUrl)), SLOT(photoTaken(KUrl)));
	
	fillKcomboDevice();
	connect(mainWidgetUi->webcamCombo,SIGNAL(currentIndexChanged(int)),SLOT(webcamChanged(int)));
	
	
//Second row Stuff
	m_modes.append(new PhotoShootMode(this));
	m_modes.append(new BurstShootMode(this));
	m_modes.append(new VideoShootMode(this));
	
	QHBoxLayout *modesLayout = new QHBoxLayout(mainWidgetUi->modes);
	
	foreach(ShootMode* mode, m_modes) {
		m_modesRadio += new QPushButton(mainWidgetUi->modes);
		m_modesRadio.last()->setIcon(mode->icon());
		m_modesRadio.last()->setIconSize(QSize(20,20));
		m_modesRadio.last()->setCheckable(true);
		m_modesRadio.last()->setAutoExclusive(true);
		m_modesRadio.last()->setToolTip(mode->name());
		modesLayout->addWidget(m_modesRadio.last());
		
		connect(m_modesRadio.last(), SIGNAL(clicked(bool)), SLOT(changeMode(bool)));
	}
	m_modesRadio.first()->setChecked(true);
	changeMode(true);
	
	mainWidgetUi->configure->setIcon(KIcon("configure"));
	connect(mainWidgetUi->configure, SIGNAL(clicked(bool)), SLOT(settingsMenu(bool)));
	
	//Third row
	//Dir operator will show the previews
	thumbnailView = new ThumbnailView(this);
	thumbnailView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	thumbnailView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	dirOperator = new KDirOperator(saveUrl, this);
	dirOperator->setMinimumHeight(100);
	dirOperator->setInlinePreviewShown(true);
	dirOperator->setIconsZoom(50);
	dirOperator->setMimeFilter(m_modes.first()->thumbnailsViewMimeTypes());
	dirOperator->updateDir();
	dirOperator->setView(thumbnailView);

	dirOperator->actionCollection()->action("by date")->trigger();
	connect(dirOperator, SIGNAL(contextMenuAboutToShow(KFileItem,QMenu*)),
			this, SLOT(contextMenuThumbnails(KFileItem,QMenu*)));
	
	//Tunning a bit the customIconView
	thumbnailView->assignDelegate();
	connect(thumbnailView, SIGNAL(doubleClicked(QModelIndex)),
			SLOT(openThumbnail(QModelIndex)));
	connect(thumbnailView->model(), SIGNAL(rowsInserted(QModelIndex, int, int)),
			SLOT(thumbnailAdded()));
	mainWidgetUi->thirdRow->insertWidget(1, dirOperator);
	
	//Arrows
	mainWidgetUi->scrollLeft->setIcon(KIcon("arrow-left"));
	mainWidgetUi->scrollRight->setIcon(KIcon("arrow-right"));
	mainWidgetUi->scrollLeft->setText(QString());
	mainWidgetUi->scrollRight->setText(QString());
	connect(mainWidgetUi->scrollLeft, SIGNAL(tick()), SLOT(slotScrollLeft()));
	connect(mainWidgetUi->scrollLeft, SIGNAL(finished()), SLOT(slotScrollFinish()));
	connect(mainWidgetUi->scrollRight, SIGNAL(tick()), SLOT(slotScrollRight()));
	connect(mainWidgetUi->scrollRight, SIGNAL(finished()), SLOT(slotScrollFinish()));
	
	whiteWidgetManager = new WhiteWidgetManager(this);
	mainWidgetUi->thirdRow->addWidget(m_countdown);
	
	connect(m_countdown, SIGNAL(finished()), SLOT(takePhoto()));
	const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
	player = Phonon::createPlayer(Phonon::NotificationCategory);
	player->setCurrentSource(soundFile);
	
	//TODO: find a better place to init this 
	m_exponentialValue = 0;
	this->setCentralWidget(mainWidget);
	
	KamosoJobTracker* tracker=new KamosoJobTracker(statusBar());
	connect(PluginManager::self(), SIGNAL(jobAdded(KamosoJob*)), tracker, SLOT(registerJob(KamosoJob*)));
	connect(tracker, SIGNAL(jobClicked(KamosoJob*)), SLOT(selectJob(KamosoJob*)));
	statusBar()->addWidget(tracker);

	#ifdef HAVE_NEPOMUK
		Nepomuk::ResourceManager::instance()->init();
	#endif
}

void Kamoso::webcamAdded()
{
	qDebug() << "A new webcam has been added";
	
	bool comboShown=deviceManager->numberOfDevices()>1;
	
	mainWidgetUi->chooseWebcamLbl->setVisible(comboShown);
	mainWidgetUi->webcamCombo->setVisible(comboShown);
	
	if(comboShown)
		fillKcomboDevice();
}

void Kamoso::startVideo(bool sound)
{
	webcam->recordVideo(sound);
}

void Kamoso::stopVideo()
{
	KUrl finalPath = saveUrl;
	finalPath.addPath(QString("kamoso_%1.ogv").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")));
	webcam->stopRecording(finalPath);
	webcam->playFile(deviceManager->playingDevicePath());
}

void Kamoso::fillKcomboDevice()
{
	mainWidgetUi->webcamCombo->clear();
	QList <Device> deviceList = deviceManager->devices();
	QList <Device>::const_iterator i, iEnd=deviceList.constEnd();
	for(i=deviceList.constBegin();i!=iEnd;++i)
	{
		mainWidgetUi->webcamCombo->addItem(i->description(),
											i->udi());
		//If kamoso is using this device, set it as currentIndex
		if(i->udi() == deviceManager->playingDeviceUdi())
		{
			mainWidgetUi->webcamCombo->setCurrentIndex(mainWidgetUi->webcamCombo->count() -1);
		}
	}
	
}
void Kamoso::webcamRemoved()
{
	if((deviceManager->numberOfDevices()-1) < 2){
		//At the moment there are only 2 widgets to hidden, maybe a container is needed here.
		mainWidgetUi->chooseWebcamLbl->hide();
		mainWidgetUi->webcamCombo->hide();
	}else{
		//The combo is already shown (should be),so onlyupdate the content is required.
		fillKcomboDevice();
	}
}

void Kamoso::webcamChanged(int index)
{
	QString udi = mainWidgetUi->webcamCombo->itemData(index).toString();
	deviceManager->webcamPlaying(udi);

	webcam->playFile(deviceManager->playingDevicePath());
}

void Kamoso::checkInitConfig()
{
	//If kamoso doesn't know where to save the taken photos, ask for it
	if(!Settings::saveUrl().isEmpty()) {
		saveUrl = Settings::saveUrl();
	} else {
		KDirSelectDialog dirs;
		dirs.showButton(KDialog::Cancel,false);
		if(dirs.exec() && dirs.url().isValid()) {
			saveUrl = dirs.url();
			Settings::setSaveUrl(saveUrl);
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
	dialog->addPage(widgetPage,i18n("General"),"configure");
	connect(dialog,SIGNAL(settingsChanged(const QString &)), this, SLOT(generalUpdated())); 

	Ui::pictureConfigWidget *pagePicture = new Ui::pictureConfigWidget;
	QWidget *widgetPicturePage = new QWidget();
	pagePicture->setupUi(widgetPicturePage);
	pagePicture->kcfg_photoTime->setValue(Settings::photoTime());
	dialog->addPage(widgetPicturePage,i18n("Photo Settings"),"insert-image");
	
	//TODO: Use the designer and so on
	KPluginSelector* selector=new KPluginSelector(dialog);
// 	selector->
	selector->addPlugins(PluginManager::self()->pluginInfo());
	dialog->addPage(selector, i18n("Plugin List"), "preferences-plugin");
	
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
	delete m_countdown;
	delete dirOperator;
	Settings::self()->writeConfig();
}

/**
*When Take Picture button is pushed, this slot is called
*/
//TODO: Abstraction of what is called on pushBtn?
void Kamoso::startCountdown(qreal minimumTime)
{
	qDebug() << Settings::photoTime();
	int time = qMax(minimumTime, 1000.*Settings::photoTime());
	
	m_countdown->start(time);
	//hidding all non-semaphore widgets
	mainWidgetUi->scrollLeft->hide();
	mainWidgetUi->scrollRight->hide();
	thumbnailView->hide();
	dirOperator->hide();
	m_countdown->show();
}

/**
*This Method is called when the countDown is over
*/
void Kamoso::takePhoto()
{
	stopCountdown();
	
	if(m_flashEnabled){
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

void Kamoso::stopCountdown()
{
	mainWidgetUi->scrollLeft->show();
	mainWidgetUi->scrollRight->show();
	dirOperator->show();
	thumbnailView->show();
	m_countdown->hide();
}

/**
*This is called after sempahore has end
*/
void Kamoso::restore()
{
	whiteWidgetManager->hideAll();
	if(m_flashEnabled) {
		Solid::Control::PowerManager::setBrightness(brightBack);
	}
}

void Kamoso::slotScrollLeft()
{
	int v=thumbnailView->horizontalScrollBar()->value();
	int min=thumbnailView->horizontalScrollBar()->minimum();
	int max=thumbnailView->horizontalScrollBar()->maximum();
	thumbnailView->horizontalScrollBar()->setValue(qBound(min, v-m_exponentialValue, max));
	
	//If this code becomes 1 line larger, export it to a method
	if(m_exponentialValue < max_exponential_value){
		m_exponentialValue += exponential_increment;
	}
}

void Kamoso::slotScrollRight()
{
	int v=thumbnailView->horizontalScrollBar()->value();
	int min=thumbnailView->horizontalScrollBar()->minimum();
	int max=thumbnailView->horizontalScrollBar()->maximum();
	thumbnailView->horizontalScrollBar()->setValue(qBound(min, v+m_exponentialValue, max));
	
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
	QString filename;
	if(idx.isValid())
		filename=idx.data(Qt::DisplayRole).toString();
	else if(!thumbnailView->selectionModel()->selectedIndexes().isEmpty()) {
		QModelIndex aux=thumbnailView->selectionModel()->selectedIndexes().first();
		filename=aux.data(Qt::DisplayRole).toString();
	}
	
	if (!filename.isEmpty())
	{
		KUrl path = saveUrl;
		path.addPath(filename);
		openThumbnail(QList<KUrl>() << path);
	}
}

void Kamoso::openThumbnail(const QList<KUrl>& url)
{
	PluginManager::self()->pluginFromName("Execute")->executeContextMenuAction(url);
}

void Kamoso::contextMenuThumbnails(const KFileItem& item, QMenu* menu)
{
	menu->clear();
	
	foreach(KamosoPlugin* p, PluginManager::self()->plugins()) {
		#warning make it possible to deal with many url at the same time
		QAction* action=p->thumbnailsAction(QList<KUrl>() << item.url());
		
		if(action) {
			if(!action->parent())
				action->setParent(menu);
			menu->addAction(action);
		}
	}
}

void Kamoso::thumbnailAdded()
{
	QTimer::singleShot(0, this, SLOT(selectLast()));
}

void Kamoso::selectLast()
{
	ThumbnailView* v=thumbnailView;
	v->horizontalScrollBar()->setValue(v->horizontalScrollBar()->maximum());
	
	QModelIndex idx=v->model()->index(v->model()->rowCount()-1, 0);
	
	if(idx.isValid())
		v->selectionModel()->setCurrentIndex(idx,
							QItemSelectionModel::Clear|QItemSelectionModel::Select);
}

void Kamoso::selectJob(KamosoJob* job)
{
	QStringList urls;
	foreach(const KUrl& url, job->urls())
		urls.append(url.pathOrUrl());
	
	dirOperator->setCurrentItems(urls);
}

void Kamoso::changeMode(bool pressed)
{
	if(!pressed)
		return;

	QPushButton* tb=qobject_cast<QPushButton*>(sender());
	if(!tb)
		tb=m_modesRadio.first();
	
	int i=0;
	foreach(QPushButton* butt, m_modesRadio) {
		if(butt==tb)
			break;
		i++;
	}
	Q_ASSERT(i<m_modesRadio.size());

	if(m_activeMode) {
		m_activeMode->deactivate();
	}
	m_activeMode=m_modes[i];
	if(dirOperator) {
		dirOperator->setMimeFilter(m_activeMode->thumbnailsViewMimeTypes());
		dirOperator->updateDir();
	}
	QWidget* w=m_activeMode->mainAction();
	w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
	
	QHBoxLayout* v=qobject_cast<QHBoxLayout*>(mainWidgetUi->actions->layout());
	delete v->takeAt(1)->widget();
	
	v->insertWidget(1, w);
	w->setFocus();

}

CountdownWidget * Kamoso::countdown() const
{
	return m_countdown;
}

void Kamoso::settingsMenu(bool )
{
	QList<QAction*> actions=m_activeMode->actions();
	QMenu m;
	if(!actions.isEmpty()) {
		m.addActions(actions);
		m.addSeparator();
	}
	m.addAction(KIcon("configure"), i18n("Settings"), this, SLOT(configuration()));
// 	m.addAction(KIcon("about"), i18n("About Kamoso..."), this, SLOT(showAboutApplication()));
	
	m.exec(mapToGlobal(mainWidgetUi->configure->geometry().bottomLeft()));
}

