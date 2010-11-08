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

#include "webcamwidget.h"
#include "config-nepomuk.h"
#include "device.h"

#include <QtCore/qtimer.h>
#include <QtCore/qdatetime.h>
#include <QtCore/QDir>
#include <QtCore/qdebug.h>
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
#include <phonon/phononnamespace.h>
#include <phonon/objectdescription.h>
#include <phonon/objectdescriptionmodel.h>
#include <phonon/backendcapabilities.h>
#include <klocalizedstring.h>
#include <kjob.h>

#ifdef HAVE_NEPOMUK
    #include <Nepomuk/ResourceManager>
    #include <Nepomuk/Resource>
    #include <Nepomuk/Tag>
#endif

#include <QtGstreamer/QGst/Pipeline>
#include <QtGstreamer/QGst/Element>
#include <QtGstreamer/QGst/Parse>
#include <QtGstreamer/QGst/Buffer>
#include <QtGstreamer/QGst/Pad>
#include <QtGstreamer/QGst/ElementFactory>
#include <QtGstreamer/QGst/VideoOrientation>
#include <QtGstreamer/QGlib/Signal>
#include <QtGstreamer/QGst/Structure>
#include <QtGstreamer/QGst/Clock>
#include <gst/gst.h>
#include <gst/video/video.h>

void callback()
{

}

struct WebcamWidget::Private
{
    QByteArray videoTmpPath;
    QString playingFile;
    QStringList effects;
    Device device;
    KUrl destination;
    int brightness;

    QGlib::SignalHandler  m_takePhotoSignal;
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
    QGst::init();
}

void WebcamWidget::paintEvent(QPaintEvent *p_event)
{
    // FIXME this makes the video flicker
    // Make everything backgroundRole color
    //afiestas: ATM our vout is not resizable, so we dont't care
//     QPainter painter(this);
//     painter.eraseRect(rect());
}

WebcamWidget::~WebcamWidget()
{
    d->m_pipeline->setState(QGst::StateNull);
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
    qDebug() << "playFile called" << device.path();;
    setDevice(device);
    if (!d->m_pipeline.isNull()) {
        d->m_pipeline->setState(QGst::StateNull);
    }

    d->m_pipeline = QGst::Pipeline::create();
    QByteArray desc;
    qDebug() << GST_VIDEO_CAPS_xRGB_HOST_ENDIAN;
    desc.append("v4l2src ! video/x-raw-yuv, framerate=15/1 ! gamma name=gamma ! videobalance name=videoBalance ! tee name=duplicate ! queue ! xvimagesink name=videosink duplicate. ! queue name=linkQueue ! ffmpegcolorspace !");
    desc.append(GST_VIDEO_CAPS_xRGB_HOST_ENDIAN);
    desc.append("! fakesink name=fakesink");

    d->m_bin = QGst::Bin::fromDescription(desc.data());
    d->m_pipeline->add(d->m_bin);

    setVideoSettings();
    setVideoSink(d->m_bin->getElementByName("videosink"));

    d->m_pipeline->setState(QGst::StatePlaying);
}

void WebcamWidget::setDevice(const Device &device)
{
    d->device = device;
    d->playingFile = device.path();
    d->brightness = device.brightness();
}

bool WebcamWidget::takePhoto(const KUrl &dest)
{
    d->destination = dest;
    d->m_bin->getElementByName("fakesink");
    d->m_bin->getElementByName("fakesink")->setProperty("signal-handoffs", true);
    d->m_takePhotoSignal = QGlib::Signal::connect(d->m_bin->getElementByName("fakesink"), "handoff", this, &WebcamWidget::photoGstCallback);
    return true;
}

void WebcamWidget::photoGstCallback(QGst::BufferPtr buffer, QGst::PadPtr)
{
    qDebug() << "Photo callback";

    qDebug() << "C API";
    QImage img;
    //C API
    {
        GstBuffer *nativeBuffer = buffer;
        GstCaps *cap = gst_buffer_get_caps(nativeBuffer);
        if (cap) {
            qDebug() << "We've got a caps in here";
            GstStructure *structure = gst_caps_get_structure(cap, 0);
            if (structure) {
                gint width = 0;
                gint height = 0;

                gst_structure_get_int(structure, "width", &width);
                gst_structure_get_int(structure, "height", &height);
                qDebug() << "Soze: " << width << "x" << height;
                qDebug() << "Name: " << gst_structure_get_name(structure);

                if (qstrcmp(gst_structure_get_name(structure), "video/x-raw-yuv") == 0) {
                        guint32 fourcc = 0;
                        gst_structure_get_fourcc(structure, "format", &fourcc);

                        if (fourcc == GST_MAKE_FOURCC('I','4','2','0')) {
                            img = QImage(width/2, height/2, QImage::Format_RGB32);

                            const uchar *data = (const uchar *)nativeBuffer->data;

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
                            qDebug() << "I'm not that weird thing";
                        }

                    } else if (qstrcmp(gst_structure_get_name(structure), "video/x-raw-rgb") == 0) {
                        qDebug() << "RGB name";
                        QImage::Format format = QImage::Format_Invalid;
                        int bpp = 0;
                        gst_structure_get_int(structure, "bpp", &bpp);

                        if (bpp == 24)
                            format = QImage::Format_RGB888;
                        else if (bpp == 32)
                            format = QImage::Format_RGB32;

                        if (format != QImage::Format_Invalid) {
                            img = QImage((const uchar *)nativeBuffer->data,
                                         width,
                                         height,
                                         format);
                            img.bits(); //detach
                        }
                    }
            }
        }
        qDebug() << "Image bytecount: " << img.byteCount();
        img.save(d->destination.path());
        d->m_bin->getElementByName("fakesink")->setProperty("signal-handoffs", false);
        d->m_takePhotoSignal.disconnect();
    }

//     C++ API
//     qDebug() << "C++ API";
//     {
//        QGst::SharedStructure structure = buffer->caps()->structure(0);
//        if (structure.isValid()) {
//            qDebug() << "Structure OK";
//            qDebug() << "Structure" << structure.toString();
//            qDebug() << "Width: " << structure.value("width").get<gint>();
//        }
//     }
}

void WebcamWidget::fileSaved(const KUrl &dest) {
    #ifdef HAVE_NEPOMUK
        if(Nepomuk::ResourceManager::instance()->initialized()) {
            qDebug() << dest;
            Nepomuk::Tag tag("kamoso");
            Nepomuk::Resource file(QUrl(dest.toLocalFile()));
            file.addTag(tag);
            file.addTag(tag);//Maybe is my computer, but I need to do ths twice, I'll it investigate later
        }
    #endif
}

void WebcamWidget::fileSaved(KJob *job)
{
    KIO::CopyJob *copy = static_cast<KIO::CopyJob *>(job);
    fileSaved(copy->destUrl());
}

void WebcamWidget::recordVideo(bool sound)
{
    d->videoTmpPath = QString(QDir::tempPath() + "/kamoso_%1.mkv").arg(QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss")).toAscii();
    qDebug() << "Record video";
    QByteArray str = "v4l2src ! video/x-raw-yuv, framerate=15/1 ! gamma name=gamma ! videobalance name=videoBalance ! tee name=duplicate ! queue ! xvimagesink name=videosink duplicate. ! queue ! theoraenc ! queue ! mux. alsasrc ! audio/x-raw-int,rate=48000,channels=2,depth=16 ! queue ! audioconvert ! queue ! vorbisenc ! queue ! mux. matroskamux name=mux ! filesink location=";
    str.append(d->videoTmpPath);
    QGst::BinPtr bin = QGst::Bin::fromDescription(str.data());
    d->m_pipeline->setState(QGst::StateNull);

    d->m_pipeline->remove(d->m_bin);
    d->m_pipeline->add(bin);
    d->m_bin = bin;

    setVideoSink(bin->getElementByName("videosink"));
    setVideoSettings();
    d->m_pipeline->setState(QGst::StatePlaying);
}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
    KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
    connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
    job->setAutoDelete(true);
    job->start();
}

#if PHONON_VERSION < PHONON_VERSION_CHECK(4, 4, 3)
namespace Phonon {
    typedef QPair<QByteArray, QString> DeviceAccess;
    typedef QList<DeviceAccess> DeviceAccessList;
}
Q_DECLARE_METATYPE(Phonon::DeviceAccessList)
#endif

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
    return 0.0;
}