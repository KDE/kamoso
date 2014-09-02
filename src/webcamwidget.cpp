/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#include "webcamwidget.h"
#include "device.h"

#include <QtCore/qtimer.h>
#include <QtCore/qdatetime.h>
#include <QtCore/QDir>
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qslider.h>
#include <QtGui/qframe.h>
#include <QtGui/qpainter.h>

#include <kurl.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <ktemporaryfile.h>
#include <kio/copyjob.h>
#include <kdebug.h>
#include <phonon/phononnamespace.h>
#include <phonon/objectdescription.h>
#include <phonon/objectdescriptionmodel.h>
#include <phonon/backendcapabilities.h>
#include <klocalizedstring.h>
#include <kjob.h>

#include <QGst/Pipeline>
#include <QGst/Element>
#include <QGst/Parse>
#include <QGst/Buffer>
#include <QGst/Pad>
#include <QGst/Fourcc>
#include <QGst/ElementFactory>
#include <QGst/VideoOrientation>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Structure>
#include <QGst/Clock>
#include <QGst/Init>
#include <QGst/VideoOverlay>
#include <QGst/Message>
#include <QGst/Memory>
#include <QGst/Bus>

struct WebcamWidget::Private
{
    QByteArray videoTmpPath;
    QString playingFile;
    QStringList effects;
    Device device;
    KUrl destination;
    int brightness;
    bool m_recording;

    QGst::PipelinePtr m_pipeline;
    QGst::BinPtr m_bin;
};

WebcamWidget *WebcamWidget::s_instance = NULL;

WebcamWidget* WebcamWidget::createInstance(QWidget *parent)
{
    if(s_instance == NULL) {
        s_instance = new WebcamWidget(parent);
    }
    return s_instance;
}

WebcamWidget* WebcamWidget::self()
{
    //TODO: add error reporting if WebcamWidget is not instance
    return s_instance;
}

WebcamWidget::WebcamWidget(QWidget* parent)
    : QGst::Ui::VideoWidget(parent), d(new Private)
{
    d->m_recording = false;
    QGst::init();

    d->m_pipeline = QGst::Pipeline::create();
    d->m_pipeline->bus()->addSignalWatch();
    QGlib::connect(d->m_pipeline->bus(), "message::error", this, &WebcamWidget::onBusMessage);

    watchPipeline(d->m_pipeline);
}

WebcamWidget::~WebcamWidget()
{
    d->m_pipeline->setState(QGst::StateNull);
    stopPipelineWatch();
}

void WebcamWidget::setVideoSettings()
{
    setBrightness(d->device.brightness());
    setContrast(d->device.contrast());
    setSaturation(d->device.saturation());
    setGamma(d->device.gamma());
    setHue(d->device.hue());
}

void WebcamWidget::playFile(const Device &device)
{
    kDebug() << "Displaying device:" << device.path();
    if (device.path().isEmpty()) {
        return;
    }
    setDevice(device);

    QByteArray pipe = basicPipe();

    //Set the right colorspace to convert to QImage
    pipe += " ! fakesink name=fakesink";

    kDebug() << "================ PIPELINE ================";
    kDebug() << pipe;

    if (d->m_pipeline->currentState() != QGst::StateNull) {
        d->m_pipeline->setState(QGst::StateNull);
    }
    if (!d->m_bin.isNull()) {
        d->m_pipeline->remove(d->m_bin);
    }

    try {
        d->m_bin = QGst::Bin::fromDescription(pipe.constData());
    } catch (const QGlib::Error & error) {
        qWarning() << "error" << error;
        return;
    }
    d->m_pipeline->add(d->m_bin);
    d->m_pipeline->setState(QGst::StateReady);

    activeAspectRatio();
    setVideoSettings();

    kDebug() << "================ Capabilities ================";
//     kDebug() << d->m_pipeline->getElementByName("v4l2src")->getStaticPad("src")->currentCaps()->toString(); //commented out for now: src-pad returns null
    d->m_pipeline->setState(QGst::StatePlaying);
}

void WebcamWidget::setDevice(const Device &device)
{
    if (device.path().isEmpty()) {
        return;
    }
    kDebug() << device.udi();
    d->device = device;
    d->playingFile = device.path();
    d->brightness = device.brightness();
}

bool WebcamWidget::takePhoto(const KUrl &dest)
{
    if (d->device.path().isEmpty()) {
        return false;
    }
    kDebug() << dest;
    d->destination = dest;
    if (!d->m_bin) {
        return false;
    }
    QGst::ElementPtr fakesink = d->m_bin->getElementByName("fakesink");
    Q_ASSERT(fakesink);
    fakesink->setProperty("signal-handoffs", true);
    QGlib::connect(fakesink, "handoff", this, &WebcamWidget::photoGstCallback);
    return true;
}

//This code has been borrowed from the Qt Multimedia project.
void WebcamWidget::photoGstCallback(QGst::BufferPtr buffer, QGst::PadPtr pad)
{
    kDebug();

    QImage img;
    QGst::CapsPtr caps = pad->currentCaps();

    const QGst::StructurePtr structure = caps->internalStructure(0);
    int width, height;
    QString format;
    format = structure->value("format").get<QString>();
    width = structure->value("width").get<int>();
    height = structure->value("height").get<int>();
    kDebug() << "We've got a caps in here";
    kDebug() << "Size: " << width << "x" << height;
    kDebug() << "Name: " << structure->name();
    kDebug() << "Format: " << format;

    if (format == "I420") {
        QGst::Fourcc fourcc = structure->value("format").get<QGst::Fourcc>();

        img = QImage(width/2, height/2, QImage::Format_RGB32);

        QGst::MemoryPtr memory;
        QGst::MapInfo info;
        memory = buffer->getMemory(0);
        if (!memory) {
            kError() << "Could not get memory for buffer.";
            return;
        }
        if (!memory->map(info, QGst::MapRead)) {
            kError() << "Could not map memory buffer.";
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
        kDebug() << "RGB name";
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
                kError() << "Could not get memory for buffer.";
                return;
            }
            if (!memory->map(info, QGst::MapRead)) {
                kError() << "Could not map memory buffer.";
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

    kDebug() << "Image bytecount: " << img.byteCount();
    img.save(d->destination.path());
    emit fileSaved(d->destination);

    d->m_bin->getElementByName("fakesink")->setProperty("signal-handoffs", false);
    QGlib::disconnect(d->m_bin->getElementByName("fakesink"), "handoff", this, &WebcamWidget::photoGstCallback);
}

QSize WebcamWidget::sizeHint() const
{
    return QSize(533, 350);
}

void WebcamWidget::fileSaved(KJob *job)
{
    KIO::CopyJob *copy = static_cast<KIO::CopyJob *>(job);
    emit fileSaved(copy->destUrl());
}

void WebcamWidget::recordVideo(bool sound)
{
    if (d->device.path().isEmpty()) {
        return;
    }
    d->videoTmpPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")).toAscii();
    kDebug() << d->videoTmpPath;
    kDebug() << "Sound: " << sound;

    QByteArray pipe = basicPipe();

    pipe +=
        //Use THEORA as video codec
        " ! theoraenc"
        " ! queue"
        //Get the audio from alsa
        " ! mux. autoaudiosrc "
        //Sound type and quality
        " ! audio/x-raw,rate=48000,channels=2,depth=16 "
        //Encode sound as vorbis
        " ! queue ! audioconvert ! queue "
        " ! vorbisenc "
        " ! queue "
        //Save everything in a matroska container
        " ! mux. matroskamux name=mux "
        //Save file in...
        " ! filesink location=";

    pipe += d->videoTmpPath;
    kDebug() << pipe;

    QGst::BinPtr bin;
    try {
        bin = QGst::Bin::fromDescription(pipe.constData());
    } catch (const QGlib::Error & error) {
        kDebug() << error;
        return;
    }

    d->m_pipeline->setState(QGst::StateNull);

    d->m_pipeline->remove(d->m_bin);
    d->m_pipeline->add(bin);
    d->m_bin = bin;

    d->m_pipeline->setState(QGst::StateReady);
    activeAspectRatio();
    setVideoSettings();
    d->m_pipeline->setState(QGst::StatePlaying);

    d->m_recording = true;
}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
    if (!d->m_recording) {
        return;
    }

    kDebug() << destUrl;
    KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
    connect(job,SIGNAL(result(KJob*)),this, SLOT(fileSaved(KJob*)));
    job->setAutoDelete(true);
    job->start();

    d->m_recording = false;
}

QByteArray WebcamWidget::phononCaptureDevice()
{
    const QList<Phonon::AudioCaptureDevice> &m_modelData = Phonon::BackendCapabilities::availableAudioCaptureDevices();
    QVariant variantList =  m_modelData.first().property("deviceAccessList");
    Phonon::DeviceAccessList accessList = variantList.value<Phonon::DeviceAccessList>();

    Phonon::DeviceAccessList::const_iterator i, iEnd=accessList.constEnd();
    for(i=accessList.constBegin(); i!=iEnd; ++i) {
        if(i->first == "alsa" && !i->second.contains("phonon")) {
            return i->second.toAscii();
        }
    }

    return QByteArray();
}

QByteArray WebcamWidget::basicPipe()
{
    QByteArray pipe;

    //Video source device=/dev/video0 for example
    pipe += "v4l2src name=v4l2src device="+d->playingFile.toLatin1();

    //Accepted capabilities
    pipe +=
    " ! video/x-raw, format=I420, width=640"

    //Basic plug-in for video controls
    " ! gamma name=gamma"
    " ! videobalance name=videoBalance"

    //Pipeline fork
    " ! tee name=duplicate"

    //Video output
    " ! queue ! videoscale ! autovideosink name=videosink duplicate."

    //Queue for the rest of the pipeline which is custom for playFile and recordVideo
    " ! queue name=linkQueue";

    return pipe;
}

void WebcamWidget::setBrightness(int level)
{
    d->m_bin->getElementByName("videoBalance")->setProperty("brightness", (double) level / 100);
}

void WebcamWidget::setContrast(int level)
{
    d->m_bin->getElementByName("videoBalance")->setProperty("contrast", (double) level / 100);
}

void WebcamWidget::setSaturation(int level)
{
    d->m_bin->getElementByName("videoBalance")->setProperty("saturation", (double) level / 100);
}

void WebcamWidget::setGamma(int level)
{
    d->m_bin->getElementByName("gamma")->setProperty("gamma", (double) level / 100);
}

void WebcamWidget::setHue(int level)
{
    d->m_bin->getElementByName("videoBalance")->setProperty("hue", (double) level / 100);
}

float WebcamWidget::convertAdjustValue(int level)
{
    Q_UNUSED(level);
    return 0.0;
}

void WebcamWidget::activeAspectRatio()
{
    QGst::BinPtr sink = d->m_bin->getElementByName("videosink").staticCast<QGst::Bin>();

    QGlib::RefPointer<QGst::VideoOverlay> over =  sink->getElementByInterface<QGst::VideoOverlay>();

    if (over->findProperty("force-aspect-ratio")) {
        kDebug() << "Setting aspect ratio";
        over->setProperty("force-aspect-ratio", true);
    }
}

void WebcamWidget::onBusMessage(const QGst::MessagePtr& message)
{
    QGlib::RefPointer< QGst::ErrorMessage > msg = message.staticCast<QGst::ErrorMessage>();
    kDebug() << msg->error() << msg->debugMessage();
    return;
}
