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
#include <QtGstreamer/QGst/Parse>
#include <QtGstreamer/QGst/ElementFactory>
#include <QtGstreamer/QGst/VideoOrientation>

void callback()
{

}

struct WebcamWidget::Private
{
    QByteArray videoTmpPath;
    QString playingFile;
    QStringList effects;
    Device device;
    int brightness;

    QGst::PipelinePtr m_pipeline;
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

void WebcamWidget::playing()
{
    qDebug() << "Playing....";
    setBrightness(d->device.brightness());
    setContrast(d->device.contrast());
    setSaturation(d->device.saturation());
    setGamma(d->device.gamma());
    setHue(d->device.hue());
}

void WebcamWidget::playFile(const Device &device)
{
    qDebug() << "playFile called" << device.path();;
    d->m_pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription("v4l2src ! ffmpegcolorspace ! tee name=duplicate ! xvimagesink name=videosink duplicate.");
    d->m_pipeline->add(bin);

    setVideoSink(bin->getElementByName("videosink"));

    d->m_pipeline->setState(QGst::StatePlaying);

    setDevice(device);
}

void WebcamWidget::setDevice(const Device &device)
{
    d->device = device;
    d->playingFile = device.path();
    d->brightness = device.brightness();
}

bool WebcamWidget::takePhoto(const KUrl &dest)
{
    return true;
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

}

void WebcamWidget::stopRecording(const KUrl &destUrl)
{
    KIO::CopyJob* job=KIO::move(KUrl(d->videoTmpPath), destUrl);
    connect(job,SIGNAL(result(KJob *)),this, SLOT(fileSaved(KJob *)));
    job->setAutoDelete(true);
    job->start();
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

void WebcamWidget::setBrightness(int level)
{

}

void WebcamWidget::setContrast(int level)
{

}

void WebcamWidget::setSaturation(int level)
{

}

void WebcamWidget::setGamma(int level)
{

}

void WebcamWidget::setHue(int level)
{

}

float WebcamWidget::convertAdjustValue(int level)
{
    return 0.0;
}