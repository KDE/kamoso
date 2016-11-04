/*************************************************************************************
 *  Copyright (C) 2012 by Alejandro Fiestas Olivares <afiestaso@kde.org>             *
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


#include "webcamcontrol.h"
#include "kamosoSettings.h"
#include <devicemanager.h>
#include <kamosodirmodel.h>
#include <previewfetcher.h>
#include <whitewidgetmanager.h>
#include <kamoso.h>
#include <KIO/CopyJob>

#include <QGlib/Connect>
#include <QGlib/Signal>
#include <QGlib/Error>
#include <QGst/Caps>
#include <QGst/Pad>
#include <QGst/structure.h>
#include <QGst/buffer.h>
#include <QGst/Quick/VideoSurface>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Parse>
#include <QGst/structs.h>
#include <QGst/Bus>
#include <QGst/Init>
#include <QGst/VideoOverlay>
#include <QGst/Message>

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <qqml.h>
#include <kdeclarative/kdeclarative.h>
#include <KJob>

WebcamControl::WebcamControl()
{
    QGst::init();

    QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine);
    kdeclarative.setTranslationDomain("kamoso");
    kdeclarative.setupBindings();

    qmlRegisterUncreatableType<Device>("org.kde.kamoso", 3, 0, "Device", "You're not supposed to mess with this yo");
    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");
    qmlRegisterUncreatableType<KJob>("org.kde.kamoso", 3, 0, "KJob", "you're not supposed to do that");

//     QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface(this);
    engine->rootContext()->setContextProperty("config", Settings::self());
    engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    engine->rootContext()->setContextProperty("devicesModel", DeviceManager::self());
    engine->rootContext()->setContextProperty("webcam", new Kamoso(this));
//     engine->rootContext()->setContextProperty("videoSurface1", surface);
    engine->load(QUrl("qrc:/qml/Main.qml"));

//     m_videoSink = surface->videoSink();
//     m_videoSink->setProperty("force-aspect-ratio", true);

    connect(DeviceManager::self(), SIGNAL(playingDeviceChanged()), SLOT(play()));
    connect(DeviceManager::self(), SIGNAL(noDevices()), SLOT(stop()));

    if (DeviceManager::self()->rowCount() > 0) {
        play(DeviceManager::self()->playingDevice());
    }
}

WebcamControl::~WebcamControl()
{
    DeviceManager::self()->save();
    Settings::self()->save();
}

void WebcamControl::stop()
{
    qDebug() << "Stop";
}

void WebcamControl::play()
{
    play(DeviceManager::self()->playingDevice());
}

void WebcamControl::play(Device *device)
{
//     //If we already have a pipeline for this device, just set it to picture mode
//     if (m_pipeline && m_currentDevice == device->udi()) {
//         m_pipeline->setProperty("mode", 2);
//         m_pipeline->setProperty("location", m_tmpVideoPath);
//         return;
//     }
//
//     //If we are changing the device, cleanup and stop old pipeline
//     if (m_pipeline && m_currentDevice != device->udi()) {
//         //Should we maybe try to just change the device path instead of re-creating?
//         qDebug() << "playing device" << device->path() << m_pipeline->currentState();
//         m_pipeline->setState(QGst::StateNull);
//     }
//
//     m_gamma = bin->getElementByName("gamma");
//     m_videoBalance = bin->getElementByName("video_balance");
//
//     auto cameraSource = QGst::ElementFactory::make("wrappercamerabinsrc", "video_balance");
//     cameraSource->setProperty("video-source-filter", bin);
//     cameraSource->setProperty("video-source", source);
//
//     setVideoSettings();
//
//     m_currentDevice = device->udi();
}

void WebcamControl::takePhoto(const QUrl &url)
{
//     if (!m_pipeline) {
//         qWarning() << "couldn't take photo, no pipeline";
//         return;
//     }
//     m_pipeline->setProperty("mode", 1);
//
//     const QString path = url.isLocalFile() ? url.toLocalFile() : QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/kamoso_photo.jpg";
//     m_pipeline->setProperty("location", path);
//
//     QGlib::emit<void>(m_pipeline, "start-capture");
//
//     if (!url.isLocalFile()) {
//         KIO::copy(QUrl::fromLocalFile(path), url);
//     }
}

void WebcamControl::startRecording()
{
//     QString date = QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss");
//     m_tmpVideoPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(date);
//
//     m_pipeline->setProperty("mode", 2);
//     m_pipeline->setProperty("location", m_tmpVideoPath);
//
//     QGlib::emit<void>(m_pipeline, "start-capture");
}

QString WebcamControl::stopRecording()
{
//     QGlib::emit<void>(m_pipeline, "stop-capture");
    return m_tmpVideoPath;
}

void WebcamControl::setVideoSettings()
{
    Device *device = DeviceManager::self()->playingDevice();
    connect(device, SIGNAL(brightnessChanged(int)), SLOT(setBrightness(int)));
    connect(device, SIGNAL(hueChanged(int)), SLOT(setHue(int)));
    connect(device, SIGNAL(contrastChanged(int)), SLOT(setContrast(int)));
    connect(device, SIGNAL(gammaChanged(int)), SLOT(setGamma(int)));
    connect(device, SIGNAL(saturationChanged(int)), SLOT(setSaturation(int)));

    setBrightness(device->brightness());
    setContrast(device->contrast());
    setSaturation(device->saturation());
    setGamma(device->gamma());
    setHue(device->hue());
}

void WebcamControl::setBrightness(int level)
{
//     m_videoBalance->setProperty("brightness", (double) level / 100);
}

void WebcamControl::setContrast(int level)
{
//     m_videoBalance->setProperty("contrast", (double) level / 100);
}

void WebcamControl::setSaturation(int level)
{
//     m_videoBalance->setProperty("saturation", (double) level / 100);
}

void WebcamControl::setGamma(int level)
{
//     m_gamma->setProperty("gamma", (double) level / 100);
}

void WebcamControl::setHue(int level)
{
//     m_videoBalance->setProperty("hue", (double) level / 100);
}
