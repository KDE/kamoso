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

#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Caps>
#include <QGst/Pad>
#include <QGst/structure.h>
#include <QGst/buffer.h>
#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Parse>
#include <QGst/structs.h>
#include <QGst/Bus>

#include <gst/video/video.h>

#include <QtCore/QDir>

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include <KDebug>
#include <KUrl>

WebcamControl::WebcamControl(QDeclarativeView* view)
{
    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view);
    view->engine()->rootContext()->setContextProperty(QLatin1String("videoSurface1"), surface);

    m_videoSink = surface->videoSink();

    play();
}

WebcamControl::~WebcamControl()
{

}

void WebcamControl::play()
{
    QByteArray pipe = basicPipe();
    pipe += " ! ffmpegcolorspace ! "
            GST_VIDEO_CAPS_xRGB_HOST_ENDIAN
            " ! fakesink name=fakesink";

    try {
        m_pipeline = QGst::Parse::launch(pipe.constData()).dynamicCast<QGst::Pipeline>();
    } catch (const QGlib::Error & error) {
        qWarning("Can't create pipeline");
        return;
    }

    m_pipeline->add(m_videoSink);

    m_pipeline->getElementByName("videoPad")->link(m_videoSink);

    m_pipeline->setState(QGst::StatePlaying);
}

void WebcamControl::takePhoto(const KUrl &url)
{
    m_saveUrl = url;
    m_pipeline->getElementByName("fakesink")->setProperty("signal-handoffs", true);
    QGlib::connect(m_pipeline->getElementByName("fakesink"), "handoff", this, &WebcamControl::photoGstCallback);
}

void WebcamControl::startRecording()
{
    m_pipeline->setState(QGst::StateNull);

    QString date = QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss");
    m_tmpVideoPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(date);
    kDebug() << m_tmpVideoPath;

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
    kDebug() << pipe;

    try {
         m_pipeline = QGst::Parse::launch(pipe.constData()).dynamicCast<QGst::Pipeline>();;
    } catch (const QGlib::Error & error) {
        kDebug() << error;
        return;
    }

    m_pipeline->add(m_videoSink);

    m_pipeline->getElementByName("videoPad")->link(m_videoSink);

//     m_pipeline->setState(QGst::StateReady);
//     activeAspectRatio();
//     setVideoSettings();
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

void WebcamControl::photoGstCallback(QGst::BufferPtr buffer, QGst::PadPtr)
{
    kDebug();

    QImage img;
    QGst::CapsPtr caps = buffer->caps();

    const QGst::StructurePtr structure = caps->internalStructure(0);
    int width, height;
    width = structure.data()->value("width").get<int>();
    height = structure.data()->value("height").get<int>();
    kDebug() << "We've got a caps in here";
    kDebug() << "Size: " << width << "x" << height;
    kDebug() << "Name: " << structure.data()->name();

    if (qstrcmp(structure.data()->name().toLatin1(), "video/x-raw-yuv") == 0) {
        QGst::Fourcc fourcc = structure->value("format").get<QGst::Fourcc>();
        kDebug() << "fourcc: " << fourcc.value.as_integer;
        if (fourcc.value.as_integer == QGst::Fourcc("I420").value.as_integer) {
            img = QImage(width/2, height/2, QImage::Format_RGB32);

            const uchar *data = (const uchar *)buffer->data();

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
        } else {
            kDebug() << "Not I420";
        }

    } else if (qstrcmp(structure.data()->name().toLatin1(), "video/x-raw-rgb") == 0) {
        kDebug() << "RGB name";
        QImage::Format format = QImage::Format_Invalid;
        int bpp = structure.data()->value("bpp").get<int>();

        if (bpp == 24)
            format = QImage::Format_RGB888;
        else if (bpp == 32)
            format = QImage::Format_RGB32;

        if (format != QImage::Format_Invalid) {
            img = QImage((const uchar *)buffer->data(),
                            width,
                            height,
                            format);
            img.bits(); //detach
        }
    }

    kDebug() << "Image bytecount: " << img.byteCount();
    img.save(m_saveUrl.path());

    Q_EMIT photoTaken(m_saveUrl.url());
    m_pipeline->getElementByName("fakesink")->setProperty("signal-handoffs", false);
    QGlib::disconnect(m_pipeline->getElementByName("fakesink"), "handoff", this, &WebcamControl::photoGstCallback);
}

QByteArray WebcamControl::basicPipe()
{
    QByteArray pipe;

    //Video source device=/dev/video0 for example
    pipe += "v4l2src name=v4l2src";

    //Accepted capabilities
    pipe +=
    " ! ffmpegcolorspace"
    " ! video/x-raw-yuv, width=640, height=480, framerate=15/1;"
    " video/x-raw-yuv, width=640, height=480, framerate=24/1;"
    " video/x-raw-yuv, width=640, height=480, framerate=30/1;"
    " video/x-raw-yuv, width=352, height=288, framerate=15/1"

    //Basic plug-in for video controls
    " ! gamma name=gamma"
    " ! videobalance name=videoBalance"

    //Pipeline fork
    " ! tee name=duplicate"

    //Video output
    " ! queue ! videoscale"

    " ! ffmpegcolorspace name=videoPad duplicate."

    //Queue for the rest of the pipeline which is custom for playFile and recordVideo
    " ! queue name=linkQueue";

    return pipe;
}

