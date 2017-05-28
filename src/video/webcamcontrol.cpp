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

class PipelineItem : public QObject, public QQmlParserStatus
{
Q_OBJECT
Q_INTERFACES(QQmlParserStatus)
Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
Q_PROPERTY(QObject* surface READ surface CONSTANT)
Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
public:
    PipelineItem()
        : QObject()
        , m_surface(new QGst::Quick::VideoSurface(this))
    {
        m_surface->videoSink()->setProperty("force-aspect-ratio", true);
    }
    ~PipelineItem() {
        if (m_pipeline)
            m_pipeline->setState(QGst::StateNull);
    }

    void onBusMessage(const QGst::MessagePtr & message)
    {
        switch (message->type()) {
        case QGst::MessageEos: //End of stream. We reached the end of the file.
            setPlaying(false);
            break;
        case QGst::MessageError:  {//Some error occurred.
            const auto error = message.staticCast<QGst::ErrorMessage>();
            qCritical() << "error on:" << m_description << error->error() << error->debugMessage();
            m_pipeline->setState(QGst::StateNull);
            m_pipeline.clear();
        }   break;
        default:
            break;
        }
    }

    void classBegin() override {}
    void componentComplete() override {
        m_complete = true;
        refresh();
    }

    QGst::Quick::VideoSurface *surface() const { return m_surface; }
    QString description() const { return m_description; }
    void setDescription(const QString &desc) {
        if (m_description != desc) {
            m_description = desc;
            Q_EMIT descriptionChanged();
        }
    }

    Q_SCRIPTABLE void refresh() {
        if (!m_description.isEmpty() && m_complete) {
            if(m_pipeline)
                m_pipeline->setState(QGst::StateNull);
            try {
                m_pipeline = QGst::Parse::launch(m_description).dynamicCast<QGst::Pipeline>();
            } catch(QGlib::Error e) {
                qDebug() << "error" << e.message();
            }
            m_pipeline->add(m_surface->videoSink());
            Q_ASSERT(m_pipeline);
            auto lastItem = m_pipeline->getElementByName("last");
            Q_ASSERT(lastItem);
            lastItem->link(m_surface->videoSink());

            QGst::BusPtr bus = m_pipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &PipelineItem::onBusMessage);
        }
        setPlaying(m_playing);
    }

    void setPlaying(bool playing) {
        m_playing = playing;
        if (!m_pipeline)
            return;
        m_pipeline->setState(playing ? QGst::StatePlaying : QGst::StatePaused);
        Q_EMIT playingChanged(playing);
    }

    bool playing() const {
        return m_pipeline && m_pipeline->currentState() == QGst::StatePlaying;
    }

Q_SIGNALS:
    void playingChanged(bool playing);
    void surfaceChanged();
    void descriptionChanged();

private:
    bool m_playing = false;
    bool m_complete = false;
    QString m_description;
    QGst::PipelinePtr m_pipeline;
    QGst::Quick::VideoSurface * const m_surface;
};

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
    qmlRegisterType<PipelineItem>("org.kde.kamoso", 3, 0, "PipelineItem");
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

    connect(DeviceManager::self(), &DeviceManager::playingDeviceChanged, this, static_cast<bool(WebcamControl::*)()>(&WebcamControl::play));
    connect(DeviceManager::self(), &DeviceManager::noDevices, this, &WebcamControl::stop);
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
    auto dev = DeviceManager::self()->playingDevice();
    return !dev || play(dev);
}

bool WebcamControl::play(Device *device)
{
    Q_ASSERT(device);

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

    m_cameraSource = QGst::ElementFactory::make("wrappercamerabinsrc", "video_balance");
    // Another option here is to return true, therefore continuing with launching, but
    // in that case the application is mostly useless.
    if (m_cameraSource.isNull()) {
        qWarning() << "The webcam controller was unable to find or load wrappercamerabinsrc plugin;"
                   << "please make sure all required gstreamer plugins are installed.";
        return false;
    }

    m_cameraSource->setProperty("video-source", source);

    m_pipeline = QGst::ElementFactory::make("camerabin").dynamicCast<QGst::Pipeline>();
    auto bus = m_pipeline->bus();
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &WebcamControl::onBusMessage);

    setVideoSettings();

    m_pipeline->setProperty("camera-source", m_cameraSource);
    m_pipeline->setProperty("viewfinder-sink", m_videoSink);
    m_pipeline->setState(QGst::StateReady);
    m_pipeline->setProperty("viewfinder-caps", QGst::Caps::fromString("video/x-raw, framerate=(fraction){30/1, 15/1}, width=(int)640, height=(int)480, format=(string){ YUY2}, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive"));


    m_pipeline->setState(QGst::StatePlaying);
    
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
    m_tmpVideoPath = QDir::tempPath() + QStringLiteral("/kamoso_%1.mkv").arg(date);

    m_pipeline->setProperty("mode", 2);
    m_pipeline->setProperty("location", m_tmpVideoPath);

    QGlib::emit<void>(m_pipeline, "start-capture");
}

QString WebcamControl::stopRecording()
{
    QGlib::emit<void>(m_pipeline, "stop-capture");
    return m_tmpVideoPath;
}

void WebcamControl::setExtraFilters(const QString& extraFilters)
{
    if (extraFilters != m_extraFilters) {
        m_extraFilters = extraFilters;
        updateSourceFilter();
    }
}

void WebcamControl::updateSourceFilter()
{
    const auto prevstate = m_pipeline->currentState();
    m_pipeline->setState(QGst::StateNull);

    //videoflip: use video-direction=horiz, method is deprecated, not changing now because video-direction doesn't seem to be available on gstreamer 1.8 which is still widely used
    QString filters = QStringLiteral("videoflip method=4");
    if (!m_extraFilters.isEmpty()) {
        filters.prepend(m_extraFilters + QStringLiteral(" ! "));
    }

    qDebug() << "setting filter" << filters;
    m_cameraSource->setProperty("video-source-filter", QGst::Bin::fromDescription(filters));

    m_pipeline->setState(prevstate);
}

void WebcamControl::setVideoSettings()
{
    Device *device = DeviceManager::self()->playingDevice();
    connect(device, &Device::filtersChanged, this, &WebcamControl::setExtraFilters);

    m_extraFilters = device->filters();
    updateSourceFilter();
}

#include "webcamcontrol.moc"
