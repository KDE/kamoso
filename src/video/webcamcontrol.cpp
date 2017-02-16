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

    QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface(this);
    engine->rootContext()->setContextProperty("config", Settings::self());
    engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    engine->rootContext()->setContextProperty("devicesModel", DeviceManager::self());
    engine->rootContext()->setContextProperty("webcam", new Kamoso(this));
    engine->rootContext()->setContextProperty("videoSurface1", surface);
    engine->load(QUrl("qrc:/qml/Main.qml"));

    m_videoSink = surface->videoSink();
    m_videoSink->setProperty("force-aspect-ratio", true);

    connect(DeviceManager::self(), SIGNAL(playingDeviceChanged()), SLOT(play()));
    connect(DeviceManager::self(), SIGNAL(noDevices()), SLOT(stop()));
}

WebcamControl::~WebcamControl()
{
    DeviceManager::self()->save();
    Settings::self()->save();
}

void WebcamControl::stop()
{
    qDebug() << "Stop";
    //TODO: delete?
    if(m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        m_pipeline.clear();
    }
}

bool WebcamControl::play()
{
    return play(DeviceManager::self()->playingDevice());
}

bool WebcamControl::play(Device *device)
{
    //If we already have a pipeline for this device, just set it to picture mode
    if (m_pipeline && m_currentDevice == device->udi()) {
        m_pipeline->setProperty("mode", 2);
        m_pipeline->setProperty("location", m_tmpVideoPath);
        return true;
    }
    
    //If we are changing the device, cleanup and stop old pipeline
    if (m_pipeline && m_currentDevice != device->udi()) {
        //Should we maybe try to just change the device path instead of re-creating?
        qDebug() << "playing device" << device->path() << m_pipeline->currentState();
        m_pipeline->setState(QGst::StateNull);
    }

    auto source = QGst::Bin::fromDescription(QLatin1String("v4l2src device=") + device->path());
    auto bin = QGst::Bin::fromDescription("videobalance name=video_balance ! gamma name=gamma ! videoflip video-direction=horiz");
    m_gamma = bin->getElementByName("gamma");
    m_videoBalance = bin->getElementByName("video_balance");

    auto cameraSource = QGst::ElementFactory::make("wrappercamerabinsrc", "video_balance");
    // Another option here is to return true, therefore continuing with launching, but
    // in that case the application is mostly useless.
    if (cameraSource.isNull()) {
        qWarning() << "The webcam controller was unable to find or load wrappercamerabinsrc plugin;"
                   << "please make sure all required gstreamer plugins are installed.";
        return false;
    }

    cameraSource->setProperty("video-source-filter", bin);
    cameraSource->setProperty("video-source", source);

    m_pipeline = QGst::ElementFactory::make("camerabin").dynamicCast<QGst::Pipeline>();
    auto bus = m_pipeline->bus();
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &WebcamControl::onBusMessage);

    m_pipeline->setProperty("camera-source", cameraSource);
    m_pipeline->setProperty("viewfinder-sink", m_videoSink);
    m_pipeline->setState(QGst::StateReady);
    m_pipeline->setProperty("viewfinder-caps", QGst::Caps::fromString("video/x-raw, framerate=(fraction){30/1, 15/1}, width=(int)640, height=(int)480, format=(string){ YUY2}, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive"));
    m_pipeline->setState(QGst::StatePlaying);

    setVideoSettings();
    
    m_currentDevice = device->udi();
    return true;
}

void WebcamControl::onBusMessage(const QGst::MessagePtr &msg)
{
    switch (msg->type()) {
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        stop();
        break;
    case QGst::MessageError: //Some error occurred.
        qCritical() << msg.staticCast<QGst::ErrorMessage>()->error();
        stop();
        play();
        break;
    case QGst::MessageElement:
        if (msg->internalStructure()->name() == "image-done") {
            Q_EMIT photoTaken(msg->internalStructure()->value("filename").toString());
        }
    default:
//         qDebug() << msg->type();
//         qDebug() << msg->typeName();
//         qDebug() << msg->internalStructure()->name();
        break;
    }
}
void WebcamControl::takePhoto(const QUrl &url)
{
    if (!m_pipeline) {
        qWarning() << "couldn't take photo, no pipeline";
        return;
    }
    m_pipeline->setProperty("mode", 1);

    const QString path = url.isLocalFile() ? url.toLocalFile() : QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/kamoso_photo.jpg";
    m_pipeline->setProperty("location", path);

    QGlib::emit<void>(m_pipeline, "start-capture");

    if (!url.isLocalFile()) {
        KIO::copy(QUrl::fromLocalFile(path), url);
    }
}

void WebcamControl::startRecording()
{
    QString date = QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss");
    m_tmpVideoPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(date);

    m_pipeline->setProperty("mode", 2);
    m_pipeline->setProperty("location", m_tmpVideoPath);

    QGlib::emit<void>(m_pipeline, "start-capture");
}

QString WebcamControl::stopRecording()
{
    QGlib::emit<void>(m_pipeline, "stop-capture");
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
    m_videoBalance->setProperty("brightness", (double) level / 100);
}

void WebcamControl::setContrast(int level)
{
    m_videoBalance->setProperty("contrast", (double) level / 100);
}

void WebcamControl::setSaturation(int level)
{
    m_videoBalance->setProperty("saturation", (double) level / 100);
}

void WebcamControl::setGamma(int level)
{
    m_gamma->setProperty("gamma", (double) level / 100);
}

void WebcamControl::setHue(int level)
{
    m_videoBalance->setProperty("hue", (double) level / 100);
}
