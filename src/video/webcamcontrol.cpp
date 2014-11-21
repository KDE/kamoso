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
    qDebug() << "Displaying device:" << device->path();
    if (device->path().isEmpty()) {
        return;
    }

    QByteArray pipe = basicPipe();

    //Set the right colorspace to convert to QImage
    pipe += " ! fakesink name=fakesink";

    qDebug() << "================ PIPELINE ================";
    qDebug() << pipe;

    try {
         m_pipeline = QGst::Parse::launch(pipe.constData()).dynamicCast<QGst::Pipeline>();
    } catch (const QGlib::Error & error) {
        qDebug() << error;
        return;
    }

    if (m_pipeline->currentState() != QGst::StateNull) {
        m_pipeline->setState(QGst::StateNull);
    }
    if (!m_bin.isNull()) {
        m_pipeline->remove(m_bin);
    }

    try {
        m_bin = QGst::Bin::fromDescription(pipe.constData());
    } catch (const QGlib::Error & error) {
        qWarning() << "error" << error;
        return;
    }
    m_pipeline->add(m_bin);
    m_pipeline->setState(QGst::StateReady);

    activeAspectRatio();
    setVideoSettings();

    qDebug() << "================ Capabilities ================";
    //     qDebug() << m_pipeline->getElementByName("v4l2src")->getStaticPad("src")->currentCaps()->toString(); //commented out for now: src-pad returns null
    m_pipeline->setState(QGst::StatePlaying);
}

void WebcamControl::takePhoto(const QUrl &url)
{
    m_saveUrl = url;
    m_pipeline->getElementByName("fakesink")->setProperty("signal-handoffs", true);
    QGlib::connect(m_pipeline->getElementByName("fakesink"), "handoff", this, &WebcamControl::photoGstCallback);
}

void WebcamControl::startRecording()
{
    stop();

    QString date = QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss");
    m_tmpVideoPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(date);
    qDebug() << m_tmpVideoPath;

    QByteArray pipe = basicPipe();

    pipe +=
        //Use THEORA as video codec
        " ! theoraenc"
        " ! queue"
        //Get the audio from alsa
        " ! mux. autoaudiosrc "
        //Sound type and quality
        " ! audio/x-raw-int,rate=48000,channels=2,depth=16 "
        //Encode sound as vorbis
        " ! queue ! audioconvert ! queue "
        " ! vorbisenc "
        " ! queue "
        //Save everything in a matroska container
        " ! mux. matroskamux name=mux "
        //Save file in...
        " ! filesink location=";

    pipe += m_tmpVideoPath.toUtf8();
    qDebug() << pipe;

    try {
         m_pipeline = QGst::Parse::launch(pipe.constData()).dynamicCast<QGst::Pipeline>();;
    } catch (const QGlib::Error & error) {
        qDebug() << error;
        return;
    }

    m_pipeline->add(m_videoSink);
    m_pipeline->getElementByName("videoPad")->link(m_videoSink);

    m_pipeline->setState(QGst::StateReady);
    setVideoSettings();
    m_pipeline->setState(QGst::StatePlaying);

    m_recording = true;
}

QString WebcamControl::stopRecording()
{
    if (!m_recording) {
        return QString();
    }

    m_pipeline->setState(QGst::StateNull);
    m_recording = false;

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
    m_pipeline->getElementByName("videoBalance")->setProperty("brightness", (double) level / 100);
}

void WebcamControl::setContrast(int level)
{
    m_pipeline->getElementByName("videoBalance")->setProperty("contrast", (double) level / 100);
}

void WebcamControl::setSaturation(int level)
{
    m_pipeline->getElementByName("videoBalance")->setProperty("saturation", (double) level / 100);
}

void WebcamControl::setGamma(int level)
{
    m_pipeline->getElementByName("gamma")->setProperty("gamma", (double) level / 100);
}

void WebcamControl::setHue(int level)
{
    m_pipeline->getElementByName("videoBalance")->setProperty("hue", (double) level / 100);
}

void WebcamControl::photoGstCallback(QGst::BufferPtr buffer, QGst::PadPtr pad)
{
    QImage img;
    QGst::CapsPtr caps = pad->currentCaps();

    const QGst::StructurePtr structure = caps->internalStructure(0);
    int width, height;
    QString format;
    format = structure->value("format").get<QString>();
    width = structure->value("width").get<int>();
    height = structure->value("height").get<int>();
    qDebug() << "We've got a caps in here";
    qDebug() << "Size: " << width << "x" << height;
    qDebug() << "Name: " << structure->name();
    qDebug() << "Format: " << format;

    if (format == "I420") {
        QGst::Fourcc fourcc = structure->value("format").get<QGst::Fourcc>();

        img = QImage(width/2, height/2, QImage::Format_RGB32);

        QGst::MemoryPtr memory;
        QGst::MapInfo info;
        memory = buffer->getMemory(0);
        if (!memory) {
            qWarning() << "Could not get memory for buffer.";
            return;
        }
        if (!memory->map(info, QGst::MapRead)) {
            qWarning() << "Could not map memory buffer.";
            return;
        }
        const uchar *data = (const uchar *)info.data();

        for (int y=0; y<height; y+=2) {
            const uchar *yLine = data + y*width;
            const uchar *uLine = data + width*height + y*width/4;
            const uchar *vLine = data + width*height*5/4 + y*width/4;

            for (int x=0; x<width; x+=2) {
                const qreal Y = 1.164*(yLine[x]-16);
                const int U = uLine[x/2]-128;
                const int V = vLine[x/2]-128;

                int b = qBound(0, int(Y + 2.018*U), 255);
                int g = qBound(0, int(Y - 0.813*V - 0.391*U), 255);
                int r = qBound(0, int(Y + 1.596*V), 255);

                img.setPixel(x/2,y/2,qRgb(r,g,b));
            }
        }
        memory->unmap(info);

    } else if (qstrcmp(structure.data()->name().toLatin1(), "video/x-raw-rgb") == 0) {
        QImage::Format format = QImage::Format_Invalid;
        int bpp = structure.data()->value("bpp").get<int>();

        if (bpp == 24)
            format = QImage::Format_RGB888;
            else if (bpp == 32)
                format = QImage::Format_RGB32;

                if (format != QImage::Format_Invalid) {
                    QGst::MemoryPtr memory;
                    QGst::MapInfo info;
                    memory = buffer->getMemory(0);
                    if (!memory) {
                        qWarning() << "Could not get memory for buffer.";
                        return;
                    }
                    if (!memory->map(info, QGst::MapRead)) {
                        qWarning() << "Could not map memory buffer.";
                        return;
                    }
                    img = QImage((const uchar *)info.data(),
                                 width,
                                 height,
                                 format);
                    img.bits(); //detach
                    memory->unmap(info);
                }
    } else
        qWarning() << "Unsupported format:" << format;

    img.save(m_saveUrl.toLocalFile());
    emit fileSaved(m_saveUrl.toLocalFile());

    m_bin->getElementByName("fakesink")->setProperty("signal-handoffs", false);
    QGlib::disconnect(m_bin->getElementByName("fakesink"), "handoff", this, &WebcamControl::photoGstCallback);
}

QByteArray WebcamControl::basicPipe()
{
    QByteArray pipe;

    //Video source device=/dev/video0 for example
    pipe += "v4l2src name=v4l2src device="+DeviceManager::self()->playingDevicePath();

    //Accepted capabilities
    pipe +=
    " ! video/x-raw, format=RGB, width=640"
    " ; video/x-raw, format=I420, width=640"
    " ; video/x-raw, width=640"

    //Basic plug-in for video controls
    " ! gamma name=gamma"
    " ! videobalance name=videoBalance"


    //Pipeline fork
    " ! tee name=duplicate"
    " ! queue"

    //Video output
    " ! autovideosink name=videosink"

    " duplicate."
    //Queue for the rest of the pipeline which is custom for playFile and recordVideo
    " ! queue";

    return pipe;
}

void WebcamControl::activeAspectRatio()
{
    QGst::BinPtr sink = m_bin->getElementByName("videosink").staticCast<QGst::Bin>();

    QGlib::RefPointer<QGst::VideoOverlay> over =  sink->getElementByInterface<QGst::VideoOverlay>();

    if (over->findProperty("force-aspect-ratio")) {
        over->setProperty("force-aspect-ratio", true);
    }
}
