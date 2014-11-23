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
#include <devicemanager.h>
#include <kamosodirmodel.h>
#include <previewfetcher.h>
#include <kamososettings.h>
#include <whitewidgetmanager.h>
#include <kamoso.h>

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

#include <gst/video/video.h>

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <qqml.h>
#include <kdeclarative/kdeclarative.h>

WebcamControl::WebcamControl()
{
    QGst::init();

    QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine);
    kdeclarative.initialize();
    kdeclarative.setupBindings();

    qmlRegisterUncreatableType<Device>("org.kde.kamoso", 3, 0, "Device", "You're not supposed to mess with this yo");
    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");

    QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface(this);
    engine->rootContext()->setContextProperty("settings", new KamosoSettings);
    engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    engine->rootContext()->setContextProperty("devicesModel", DeviceManager::self());
    engine->rootContext()->setContextProperty("webcam", new Kamoso(this));
    engine->rootContext()->setContextProperty("videoSurface1", surface);
    engine->load(QUrl("qrc:/qml/Main.qml"));

    m_videoSink = surface->videoSink();
    m_videoSink->setProperty("force-aspect-ratio", true);

    connect(DeviceManager::self(), SIGNAL(playingDeviceChanged()), SLOT(play()));
    connect(DeviceManager::self(), SIGNAL(noDevices()), SLOT(stop()));

    if (DeviceManager::self()->rowCount() > 0) {
        play(DeviceManager::self()->playingDevice());
    }
}

WebcamControl::~WebcamControl()
{
    DeviceManager::self()->save();
}

void WebcamControl::stop()
{
    qDebug() << "Stop";
    //TODO: delete?
    if(m_pipeline) {
        qDebug() << "Stopping: " << m_pipeline->name();
        m_pipeline->setState(QGst::StateNull);
        m_pipeline->getElementByName("videoPad")->unlink(m_videoSink);
        m_pipeline->remove(m_videoSink);
        m_pipeline.clear();
    }
}

void WebcamControl::play()
{
    play(DeviceManager::self()->playingDevice());
}

void WebcamControl::play(Device *device)
{
    auto bin = QGst::Bin::fromDescription("videobalance name=video_balance ! gamma name=gamma");
    m_gamma = bin->getElementByName("gamma");
    m_videoBalance = bin->getElementByName("video_balance");

    auto cameraSource = QGst::ElementFactory::make("wrappercamerabinsrc", "video_balance");
    cameraSource->setProperty("video-source-filter", bin);

    m_pipeline = QGst::ElementFactory::make("camerabin").dynamicCast<QGst::Pipeline>();
    auto bus = m_pipeline->bus();;
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &WebcamControl::messageClosure);

    m_pipeline->setProperty("camera-source", cameraSource);
    m_pipeline->setProperty("viewfinder-sink", m_videoSink);
    m_pipeline->setState(QGst::StateReady);
    m_pipeline->setProperty("viewfinder-caps", QGst::Caps::fromString("video/x-raw, framerate=(fraction){30/1, 15/1}, width=(int)640, height=(int)480, format=(string){ YUY2}, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive"));
    m_pipeline->setState(QGst::StatePlaying);

    setVideoSettings();
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
        break;
    default:
//         qDebug() << msg->type();
//         qDebug() << msg->typeName();
//         qDebug() << msg->internalStructure()->name();
        break;
    }
}
void WebcamControl::takePhoto(const QUrl &url)
{
    m_pipeline->setProperty("mode", 1);
    m_pipeline->setProperty("location", url.toLocalFile());
    QGlib::emit<void>(m_pipeline, "start-capture");
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
    connect(device, SIGNAL(brightnessChanged(int,int)), SLOT(setBrightness(int)));
    connect(device, SIGNAL(hueChanged(int,int)), SLOT(setHue(int)));
    connect(device, SIGNAL(contrastChanged(int,int)), SLOT(setContrast(int)));
    connect(device, SIGNAL(gammaChanged(int,int)), SLOT(setGamma(int)));
    connect(device, SIGNAL(saturationChanged(int,int)), SLOT(setSaturation(int)));

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