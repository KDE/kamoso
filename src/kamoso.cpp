/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
 *  Copyright (C) 2013      by Michael G. Hansen <mike@mghansen.de>                  *
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
#include "thumbnailview.h"
#include "whitewidget.h"
#include "webcamwidget.h"
#include "countdownwidget.h"
#include "settings.h"
#include "ui_generalConfig.h"
#include "ui_pictureConfig.h"
#include "ui_mainWidget.h"
#include "ui_videoConfig.h"
#include "whitewidgetmanager.h"
#include "devicemanager.h"
#include "kipiinterface.h"
#include "kamosoplugin.h"
#include "kamosojobtracker.h"
#include "photoshootmode.h"
#include "videoshootmode.h"
#include "burstshootmode.h"
#include "kipiaction.h"
#include "webcamdialog.h"
#include "pagewebcamconfigmanager.h"
#if KDE_IS_VERSION(4,5,85)
    #include "brightness_interface.h"
#else
    #include <solid/control/powermanager.h>
#endif

#include <QItemSelection>
#include <QLayout>
#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>

#include <KMenu>
#include <KActionCollection>
#include <KApplication>
#include <KConfigGroup>
#include <KDirLister>
#include <KDirSelectDialog>
#include <KLocale>
#include <KStandardDirs>
#include <KConfigDialog>
#include <KDebug>
#include <KPluginSelector>
#include <KMessageBox>
#include <KStatusBar>
#include <KIO/NetAccess>
#include <Phonon/MediaObject>
#include <kdeversion.h>
#include <KDirModel>
#include <kio/copyjob.h>

#include <libkipi/plugin.h>
#include <libkipi/pluginloader.h>
#include <libkipi/version.h>
#include <KPluginInfo>

const int max_exponential_value = 50;
const int exponential_increment = 5;

Kamoso::Kamoso(QWidget* parent)
    : KMainWindow(parent), m_activeMode(0), m_flashEnabled(true)
{
    dirModel = new KDirModel(this);

    m_countdown = new CountdownWidget(this);
    m_countdown->hide();

    deviceManager = DeviceManager::self();
    connect(deviceManager,SIGNAL(deviceRegistered(QString)),SLOT(webcamAdded()));
    connect(deviceManager,SIGNAL(deviceUnregistered(QString)),SLOT(webcamRemoved()));

    mainWidgetUi = new Ui::mainWidget;
    mainWidget = new QWidget(this);
    mainWidgetUi->setupUi(mainWidget);

    connect(this,SIGNAL(webcamPlaying(QString)),deviceManager,SLOT(webcamPlaying(QString)));
//First row Stuff, at the moment only webcam is placed here
//Setting webcam in the first row, central spot

    m_webcam = WebcamWidget::createInstance(mainWidgetUi->centralSpot);
    mainWidgetUi->centralSpot->layout()->addWidget(m_webcam);
    m_webcam->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_webcam, SIGNAL(fileSaved(KUrl)), this, SLOT(fileSaved(KUrl)));

    if(deviceManager->hasDevices()) {
        m_webcam->playFile(deviceManager->defaultDevice());
        emit webcamPlaying(deviceManager->defaultDeviceUdi());
    }

    refreshDeviceList();
    connect(mainWidgetUi->webcamCombo,SIGNAL(currentIndexChanged(int)),SLOT(webcamChanged(int)));

//Second row Stuff
    m_modes.append(new PhotoShootMode(this));
    m_modes.append(new BurstShootMode(this));
    m_modes.append(new VideoShootMode(this));

    QHBoxLayout *modesLayout = new QHBoxLayout(mainWidgetUi->modes);
    modesLayout->setSpacing(0);
    modesLayout->setContentsMargins(4, 0, 4, 0);

    foreach(ShootMode* mode, m_modes) {
        m_modesRadio += new QToolButton(mainWidgetUi->modes);
        m_modesRadio.last()->setIcon(mode->icon());
        m_modesRadio.last()->setIconSize(QSize(20,20));
        m_modesRadio.last()->setCheckable(true);
        m_modesRadio.last()->setAutoExclusive(true);
        m_modesRadio.last()->setToolTip(mode->name());
        m_modesRadio.last()->setAutoRaise(true);
        modesLayout->addWidget(m_modesRadio.last());

        connect(m_modesRadio.last(), SIGNAL(clicked(bool)), SLOT(changeMode(bool)));
    }
    m_modesRadio.first()->setChecked(true);
    changeMode(true);

    mainWidgetUi->exportFiles->setIcon(KIcon("document-export"));
    connect(mainWidgetUi->exportFiles, SIGNAL(clicked(bool)), SLOT(exportMenu(bool)));

    mainWidgetUi->configure->setIcon(KIcon("configure"));
    connect(mainWidgetUi->configure, SIGNAL(clicked(bool)), SLOT(settingsMenu(bool)));

//Third row
    //Thumbnail view will show the previews
    mainWidgetUi->thumbnailView->setModel(dirModel);
    mainWidgetUi->thumbnailView->assignDelegate();
    connect(mainWidgetUi->thumbnailView, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(openFile()));
    connect(mainWidgetUi->thumbnailView->model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            SLOT(thumbnailAdded()));
    connect(mainWidgetUi->thumbnailView->horizontalScrollBar(), SIGNAL(valueChanged(int)), SLOT(thumbnailViewMoved(int)));
    mainWidgetUi->thirdRow->insertWidget(1, mainWidgetUi->thumbnailView);
    connect(mainWidgetUi->thumbnailView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                                                            SLOT(fileViewSelectionChanged(QItemSelection,QItemSelection)));
    //Arrows
    mainWidgetUi->scrollLeft->setIcon(KIcon("arrow-left"));
    mainWidgetUi->scrollRight->setIcon(KIcon("arrow-right"));
    mainWidgetUi->scrollLeft->setText(QString());
    mainWidgetUi->scrollRight->setText(QString());
    connect(mainWidgetUi->scrollLeft, SIGNAL(clicked(bool)), SLOT(slotScrollLeft()));
    connect(mainWidgetUi->scrollRight, SIGNAL(clicked(bool)), SLOT(slotScrollRight()));

    whiteWidgetManager = new WhiteWidgetManager(this);
    mainWidgetUi->thirdRow->addWidget(m_countdown);

    connect(m_countdown, SIGNAL(finished()), SLOT(takePhoto()));
    const KUrl soundFile = KStandardDirs::locate("sound", "KDE-Im-User-Auth.ogg");
    player = Phonon::createPlayer(Phonon::NotificationCategory);
    player->setCurrentSource(soundFile);

    //TODO: find a better place to init this
    m_exponentialValue = 0;
    this->setCentralWidget(mainWidget);

    mTracker = new KamosoJobTracker(statusBar());
    connect(mTracker, SIGNAL(jobClicked(KJob*,KUrl::List)), SLOT(selectJob(KJob*,KUrl::List)));
    statusBar()->addWidget(mTracker);

    connect(mTracker, SIGNAL(urlsChanged(KUrl::List)), SLOT(updateThumbnails(KUrl::List)));

    QMetaObject::invokeMethod(this, "initialize");
#if (KIPI_VERSION >= 0x020000)
    mPluginLoader = new KIPI::PluginLoader();
    mPluginLoader->setInterface(new KIPIInterface(this));
    mPluginLoader->init();
#else
    mPluginLoader = new KIPI::PluginLoader(QStringList(), new KIPIInterface(this), "");
#endif
}

KUrl::List Kamoso::selectedItems()
{
    KUrl::List urls;
    foreach(const QModelIndex& idx, mainWidgetUi->thumbnailView->selectionModel()->selectedIndexes()) {
        urls += dirModel->itemForIndex(idx).url();
    }

    return urls;
}

void Kamoso::initialize()
{
    //Check the initial and basic config, and ask for it it doesn't exist
    checkInitConfig();

    kDebug() << "Settings of kamoso:";
    kDebug() << "saveUrl: " << Settings::saveUrl();
    kDebug() << "photoTime: " << Settings::photoTime();
}

void Kamoso::webcamAdded()
{
    kDebug() << "A new webcam has been added";

    refreshDeviceList();
}

void Kamoso::startVideo(bool sound)
{
    m_webcam->recordVideo(sound);
}

void Kamoso::stopVideo()
{
    KUrl finalPath = Settings::saveUrl();
    finalPath.addPath(QString("video_1.mkv"));

    while(KIO::NetAccess::exists( finalPath, KIO::NetAccess::DestinationSide, this )) {
        autoincFilename(finalPath);
    }

    m_webcam->stopRecording(finalPath);
    m_webcam->playFile(deviceManager->playingDevice());
}

void Kamoso::refreshDeviceList()
{
    const QList <Device> devices = deviceManager->devices();
    const bool shouldShowNoDevicesWarning = devices.isEmpty();
    const bool shouldShowDeviceList = devices.count() > 1;

    // We refresh the combo box even if it is not shown since we also have to find out whether the currently
    // playing webcam is still available. If it is not, we have to switch to a different one.

    // Block signals from the combo in order not to accidentally switch the webcam while updating the list.
    mainWidgetUi->webcamCombo->blockSignals(true);
    mainWidgetUi->webcamCombo->clear();

    bool currentWebcamStillAvailable = false;
    foreach(const Device& d, devices)
    {
        mainWidgetUi->webcamCombo->addItem(d.description(), d.udi());

        // If kamoso is using this device, set it as currentIndex
        if(d.udi() == deviceManager->playingDeviceUdi()) {
            mainWidgetUi->webcamCombo->setCurrentIndex(mainWidgetUi->webcamCombo->count() -1);
            currentWebcamStillAvailable = true;
        }
    }

    mainWidgetUi->webcamCombo->blockSignals(false);

    mainWidgetUi->chooseWebcamLbl->setVisible(shouldShowDeviceList);
    mainWidgetUi->webcamCombo->setVisible(shouldShowDeviceList);
    mainWidgetUi->warningLabel->setVisible(shouldShowNoDevicesWarning);

    if(!currentWebcamStillAvailable && !devices.isEmpty()) {
        // Select the first webcam. Since we before blocked signals, the
        // index 0 is probably selected already and no signal is generated.
        // Therefore we will call webcamChanged(int) manually to be sure.
        mainWidgetUi->webcamCombo->setCurrentIndex(0);
        webcamChanged(0);
    }
}

void Kamoso::webcamRemoved()
{
    // Update the list of devices. This will also switch to a different webcam
    // if the currently playing webcam was removed.
    refreshDeviceList();
}

void Kamoso::webcamChanged(int index)
{
    // the webcam combo's signals are blocked while it is (re-)populated

    // Only set the webcam to be displayed if it is not already playing.
    const QString udi = mainWidgetUi->webcamCombo->itemData(index).toString();
    if (deviceManager->playingDeviceUdi() != udi)
    {
        deviceManager->webcamPlaying(udi);

        m_webcam->playFile(deviceManager->playingDevice());
    }
}

void Kamoso::checkInitConfig()
{
    //If kamoso doesn't know where to save the taken photos, ask for it
    if(Settings::saveUrl().isEmpty()) {
        KDirSelectDialog dirs;
        dirs.showButton(KDialog::Cancel, false);

        KUrl url;
        if(dirs.exec() && dirs.url().isValid())
            url=dirs.url();
        else
            url=QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
        Settings::setSaveUrl(url);
    }
    dirModel->dirLister()->openUrl(Settings::saveUrl(), KDirLister::Reload);
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
    dialog = new WebcamDialog(this,"settings",Settings::self());
    dialog->resize(540,dialog->height());

    //Widget created with qt-designer
    //TODO: Check page and pagePicture leaking
    Ui::generalConfigWidget *page = new Ui::generalConfigWidget();
    QWidget *widgetPage = new QWidget();
    page->setupUi(widgetPage);
    page->kcfg_saveUrl->setMode(KFile::Directory);
    Q_EMIT(Settings::saveUrl());

    dialog->addPage(widgetPage,i18n("General"),"configure");
    connect(dialog,SIGNAL(settingsChanged(QString)), this, SLOT(generalUpdated()));

    Ui::pictureConfigWidget *pagePicture = new Ui::pictureConfigWidget;
    QWidget *widgetPicturePage = new QWidget();
    pagePicture->setupUi(widgetPicturePage);
    pagePicture->kcfg_photoTime->setValue(Settings::photoTime());
    pagePicture->kcfg_photoSound->setChecked(Settings::photoSound());
    dialog->addPage(widgetPicturePage,i18n("Photo Settings"),"insert-image");

    pageWebcam = new Ui::webcamConfigWidget;

    QWidget *widgetWebcamPage = new QWidget();
    pageWebcam->setupUi(widgetWebcamPage);
    dialog->addPage(widgetWebcamPage,i18n("Video Settings"),"camera-web");

    //the values are in X.X form while the sliders use integer so we device by 100;
    Device device = deviceManager->playingDevice();
    if (!device.path().isEmpty()) {
        pageWebcam->brightnessSlider->setValue(device.brightness());
        pageWebcam->contrastSlider->setValue(device.contrast());
        pageWebcam->saturationSlider->setValue(device.saturation());
        pageWebcam->gammaSlider->setValue(device.gamma());
        pageWebcam->hueSlider->setValue(device.hue());
    } else {
        widgetWebcamPage->setEnabled(false);
    }

    PageWebcamConfigManager* configManager = new PageWebcamConfigManager(pageWebcam);
    dialog->setPageWebcamConfigManager(configManager);

    connect(pageWebcam->brightnessSlider,SIGNAL(valueChanged(int)),dialog,SLOT(updateButtons()));
    connect(pageWebcam->contrastSlider,SIGNAL(valueChanged(int)),dialog,SLOT(updateButtons()));
    connect(pageWebcam->saturationSlider,SIGNAL(valueChanged(int)),dialog,SLOT(updateButtons()));
    connect(pageWebcam->gammaSlider,SIGNAL(valueChanged(int)),dialog,SLOT(updateButtons()));
    connect(pageWebcam->hueSlider,SIGNAL(valueChanged(int)),dialog,SLOT(updateButtons()));

    connect(pageWebcam->brightnessSlider,SIGNAL(valueChanged(int)),m_webcam,SLOT(setBrightness(int)));
    connect(pageWebcam->contrastSlider,SIGNAL(valueChanged(int)),m_webcam,SLOT(setContrast(int)));
    connect(pageWebcam->saturationSlider,SIGNAL(valueChanged(int)),m_webcam,SLOT(setSaturation(int)));
    connect(pageWebcam->gammaSlider,SIGNAL(valueChanged(int)),m_webcam,SLOT(setGamma(int)));
    connect(pageWebcam->hueSlider,SIGNAL(valueChanged(int)),m_webcam,SLOT(setHue(int)));

    //TODO: Use the designer and so on
    // 	KPluginSelector* selector=new KPluginSelector(dialog);
    // 	selector->addPlugins(PluginManager::self()->pluginInfo());
    // 	dialog->addPage(selector, i18n("Plugin List"), "preferences-plugin");
    dialog->show();
}

/**
*This is called automatically by KCM when the configuration is updated
*/
void Kamoso::generalUpdated()
{
    kDebug() << "Settings New\n" << Settings::saveUrl();
    Settings::self()->writeConfig();
    dirModel->dirLister()->openUrl(Settings::saveUrl(), KDirLister::Reload);

    Device device = deviceManager->playingDevice();

    device.setBrightness(pageWebcam->brightnessSlider->value());
    device.setContrast(pageWebcam->contrastSlider->value());
    device.setSaturation(pageWebcam->saturationSlider->value());
    device.setGamma(pageWebcam->gammaSlider->value());
    device.setHue(pageWebcam->hueSlider->value());
}

/**
*Destructor of Kamoso, save configuratoin and delete some stuff
*/
Kamoso::~Kamoso()
{
    delete player;
    Settings::self()->writeConfig();
}

/**
*When Take Picture button is pushed, this slot is called
*/
//TODO: Abstraction of what is called on pushBtn?
void Kamoso::startCountdown(qreal minimumTime)
{
    kDebug() << Settings::photoTime();
    int time = qMax(minimumTime, qreal(1000.*Settings::photoTime()));

    m_countdown->start(time);
    //hidding all non-semaphore widgets
    mainWidgetUi->scrollLeft->hide();
    mainWidgetUi->scrollRight->hide();
    mainWidgetUi->thumbnailView->hide();
    m_countdown->show();
}

/**
*This Method is called when the countDown is over
*/
void Kamoso::takePhoto()
{
    stopCountdown();

    if(m_flashEnabled){
        #if KDE_IS_VERSION(4,5,85)
            org::kde::Solid::PowerManagement power("org.kde.Solid.PowerManagement", "/org/kde/Solid/PowerManagement", QDBusConnection::sessionBus());
            brightBack = power.brightness().value();
            power.setBrightness(100);
        #else
            brightBack = Solid::Control::PowerManager::brightness();
            Solid::Control::PowerManager::setBrightness(100);
        #endif
        whiteWidgetManager->showAll();
    }
    QTimer::singleShot(1000, this, SLOT(restore()));

    KUrl photoPlace = Settings::saveUrl();
    photoPlace.addPath(QString("picture_1.png"));

    while(KIO::NetAccess::exists( photoPlace, KIO::NetAccess::DestinationSide, this )) {
        autoincFilename(photoPlace);
    }

    m_webcam->takePhoto(photoPlace);
    if (Settings::photoSound()) {
        player->play();
    }
}

void Kamoso::stopCountdown()
{
    mainWidgetUi->scrollLeft->show();
    mainWidgetUi->scrollRight->show();
    mainWidgetUi->thumbnailView->show();
    m_countdown->hide();
    m_countdown->stop();
}

/**
*This is called after sempahore has end
*/
void Kamoso::restore()
{
    whiteWidgetManager->hideAll();
    if(m_flashEnabled) {
        #if KDE_IS_VERSION(4,5,85)
            org::kde::Solid::PowerManagement power("org.kde.Solid.PowerManagement", "/org/kde/Solid/PowerManagement", QDBusConnection::sessionBus());
            power.setBrightness(brightBack);
        #else
            Solid::Control::PowerManager::setBrightness(brightBack);
        #endif
    }
}

void Kamoso::slotScrollLeft()
{
    int v = mainWidgetUi->thumbnailView->xValue();
    mainWidgetUi->thumbnailView->setXValue(v-mainWidgetUi->thumbnailView->width());
}

void Kamoso::slotScrollRight()
{
    int v = mainWidgetUi->thumbnailView->xValue();
    mainWidgetUi->thumbnailView->setXValue(v+mainWidgetUi->thumbnailView->width());
}

QPointer< QMenu > Kamoso::exportKIPIMenu()
{
    QPointer<QMenu> menu = new QMenu(this);
    QModelIndex idx = mainWidgetUi->thumbnailView->currentIndex();
    KFileItem item(dirModel->itemForIndex(idx));

    Q_FOREACH(KIPI::PluginLoader::Info* pluginInfo, mPluginLoader->pluginList()) {
        QStringList pluginMime=pluginInfo->service()->property("X-KIPI-Mimetypes").toStringList();

        foreach(const QString& supportedPlugin, pluginMime) {
            if(item.mimeTypePtr()->is(supportedPlugin)) {
                KipiAction* action=new KipiAction(pluginInfo, this, menu);

                menu->addAction(action);
                break;
            }
        }
    }

    if(!menu->isEmpty()) {
        menu->addSeparator();
    }

    menu->addAction(KIcon("user-trash"), i18n("Trash"), this, SLOT(removeSelection()));
    menu->addAction(KIcon("document-open"), i18n("Open..."), this, SLOT(openFile()));
    return menu;
}

void Kamoso::contextMenuEvent(QContextMenuEvent* event)
{
    if(!mainWidgetUi->thumbnailView->selectionModel()->hasSelection()) {
        return;
    }

    QPointer<QMenu> menu = exportKIPIMenu();

    menu->exec(mapToGlobal(event->pos()));

    delete menu;
}

void Kamoso::fileViewSelectionChanged(const QItemSelection& , const QItemSelection& )
{
    mainWidgetUi->exportFiles->setEnabled(mainWidgetUi->thumbnailView->selectionModel()->hasSelection());
}

void Kamoso::exportMenu(bool)
{
    QPointer<QMenu> menu = exportKIPIMenu();

    menu->exec(mainWidgetUi->exportFiles->parentWidget()->mapToGlobal(mainWidgetUi->exportFiles->geometry().bottomLeft()));
    delete menu;
}

void Kamoso::openFile()
{
    KUrl::List urls;
    foreach(const QModelIndex& idx, mainWidgetUi->thumbnailView->selectionModel()->selectedIndexes()) {
        QDesktopServices::openUrl(dirModel->itemForIndex(idx).url());
    }
}

void Kamoso::removeSelection()
{
    KUrl::List urls;
    foreach(const QModelIndex& idx, mainWidgetUi->thumbnailView->selectionModel()->selectedIndexes()) {
        urls << dirModel->itemForIndex(idx).url();
    }

    int res = KMessageBox::warningContinueCancel(0,
                                        i18np("Are you sure you want to delete this file?", "Are you sure you want to delete these %1 files?", urls.size()),
                                        i18n("Move to Trash"));

    if(res == KMessageBox::Continue) {
        KIO::CopyJob *job = KIO::trash(urls);
        mTracker->registerJob(job, urls, KIcon("user-trash"));
    }
}

void Kamoso::thumbnailAdded()
{
    QTimer::singleShot(0, this, SLOT(selectLast()));
}

void Kamoso::selectLast()
{
    ThumbnailView* v = mainWidgetUi->thumbnailView;
    v->horizontalScrollBar()->setValue(v->horizontalScrollBar()->maximum());

    QModelIndex idx = v->model()->index(v->model()->rowCount()-1, 0);

    if(idx.isValid()) {
        v->selectionModel()->setCurrentIndex(idx,
                            QItemSelectionModel::Clear|QItemSelectionModel::Select);
    }
}

void Kamoso::selectJob(KJob* , const KUrl::List& urls)
{
    mainWidgetUi->thumbnailView->selectionModel()->clearSelection();
    foreach(const KUrl&url, urls) {
        mainWidgetUi->thumbnailView->selectionModel()->select(dirModel->indexForUrl(url), QItemSelectionModel::Select);
    }
}

void Kamoso::changeMode(bool pressed)
{
    if(!pressed) {
        return;
    }
    mainWidgetUi->thumbnailView->clearWaiting();

    QToolButton* tb = qobject_cast<QToolButton*>(sender());
    if(!tb) {
        tb = m_modesRadio.first();
    }

    int i=0;
    foreach(QToolButton* butt, m_modesRadio) {
        if(butt == tb)
            break;
        i++;
    }
    Q_ASSERT(i < m_modesRadio.size());

    if(m_activeMode) {
        m_activeMode->deactivate();
    }
    
    if(m_activeMode==m_modes[i])
        return;
    
    m_activeMode=m_modes[i];
    dirModel->dirLister()->setMimeFilter(m_activeMode->thumbnailsViewMimeTypes());
    if(!dirModel->dirLister()->url().isEmpty())
        dirModel->dirLister()->openUrl(dirModel->dirLister()->url(), KDirLister::Reload);

    QWidget* w = m_activeMode->mainAction();
    w->setMinimumSize(54, 54);
    w->setMaximumSize(54, 54);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* v = qobject_cast<QHBoxLayout*>(mainWidgetUi->actions->layout());
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
    QList<QAction*> actions = m_activeMode->actions();
    KMenu m;
    if(!actions.isEmpty()) {
        m.addActions(actions);
        m.addSeparator();
    }
    m.addAction(KIcon("configure"), i18n("Settings"), this, SLOT(configuration()));
    KMenu *help = helpMenu();
    m.addMenu(help);

    m.exec(mainWidgetUi->configure->parentWidget()->mapToGlobal(mainWidgetUi->configure->geometry().bottomLeft()));
}

//Code taken from ksnapshot, thanks guys :p
void Kamoso::autoincFilename(KUrl &filename)
{
    // Extract the filename from the path
    QString name= filename.fileName();

    // If the name contains a number then increment it
    QRegExp numSearch( "(^|[^\\d])(\\d+)" ); // we want to match as far left as possible, and when the number is at the start of the name

    // Does it have a number?
    int start = numSearch.lastIndexIn( name );
    if (start != -1) {
        // It has a number, increment it
        start = numSearch.pos( 2 ); // we are only interested in the second group
        QString numAsStr = numSearch.cap(2);
        QString number = QString::number( numAsStr.toInt() + 1 );
        number = number.rightJustified( numAsStr.length(), '0' );
        name.replace( start, numAsStr.length(), number );
    }
    else {
        // no number
        start = name.lastIndexOf('.');
        if (start != -1) {
            // has a . somewhere, e.g. it has an extension
            name.insert(start, '1');
        }
        else {
            // no extension, just tack it on to the end
            name += '1';
        }
    }

    //Rebuild the path
    filename.setFileName( name );
}

void Kamoso::thumbnailViewMoved(int value)
{
    mainWidgetUi->scrollLeft->setEnabled(value!=0);
    mainWidgetUi->scrollRight->setEnabled(mainWidgetUi->thumbnailView->horizontalScrollBar()->maximum()!=value);
}

void Kamoso::updateThumbnails(const KUrl::List& urls)
{
    foreach(const KUrl& url, urls) {
        QModelIndex idx = dirModel->indexForUrl(url);
        QList<QIcon> icons = tracker()->iconsPerUrl(url);
        mainWidgetUi->thumbnailView->delegate()->setOverlays(url, icons);

        mainWidgetUi->thumbnailView->update(idx);
    }
}
