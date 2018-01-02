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

#include <gst/gstcaps.h>
#include <gst/gstpad.h>
#include <gst/gststructure.h>
#include <gst/gstbuffer.h>
#include <gst/gstpipeline.h>
#include <gst/gstelementfactory.h>
#include <gst/gstparse.h>
#include <gst/gstbus.h>
#include <gst/gstmessage.h>
#include <gst/gst.h>

#include "QGst/Quick/VideoItem"
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <qqml.h>
#include <KJob>
#include <KLocalizedContext>

#include "QGst/Quick/VideoSurface"

static QString debugMessage(GstMessage* msg)
{
    gchar *debug = nullptr;
    GError *e = nullptr;
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_WARNING) {
        gst_message_parse_warning(msg, &e, &debug);
    } else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        gst_message_parse_error(msg, &e, &debug);
    }
    if (!debug)
        return {};

    if (e) {
        qWarning() << "error debugMessage:" << e->message;
        g_error_free (e);
    }
    const auto ret = QString::fromUtf8(debug);
    g_free(debug);
    return ret;
}

template <class T>
GstState pipelineCurrentState(const T &pipe)
{
    GstState currentState, pendingState;
    GstStateChangeReturn result = gst_element_get_state(GST_ELEMENT(pipe.data()), &currentState, &pendingState, GST_CLOCK_TIME_NONE );
    Q_ASSERT(result != GST_STATE_CHANGE_FAILURE);
    return currentState;
}

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
        g_object_ref(m_surface->videoSink());
        g_object_set(m_surface->videoSink(), "force-aspect-ratio", true, NULL);
    }

    ~PipelineItem() {
        if (m_pipeline)
            gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);
    }

    void onBusMessage(GstMessage* message)
    {
        switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_EOS: //End of stream. We reached the end of the file.
            setPlaying(false);
            break;
        case GST_MESSAGE_ERROR:  {//Some error occurred.
            qCritical() << "error on:" << m_description << debugMessage(message);
            gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);
            m_pipeline.reset(nullptr);
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
                gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

            GError* e = nullptr;
            m_pipeline.reset(GST_PIPELINE(gst_parse_launch(m_description.toUtf8().constData(), &e)));
            if (e) {
                qWarning() << "error:" << e->message;
                Q_EMIT failed();
                return;
            }
            Q_ASSERT(m_pipeline);

            gst_bin_add(GST_BIN(m_pipeline.data()), m_surface->videoSink());
            Q_ASSERT(m_pipeline);

            auto lastItem = gst_bin_get_by_name(GST_BIN(m_pipeline.data()), "last");
            Q_ASSERT(lastItem);
            bool b = gst_element_link(lastItem, m_surface->videoSink());
            Q_ASSERT(b);
            gst_bus_add_watch (gst_pipeline_get_bus(m_pipeline.data()), &pipelineWatch, this);
        }
        setPlaying(m_playing);
    }

    static gboolean pipelineWatch(GstBus     */*bus*/, GstMessage *message, gpointer user_data)
    {
        PipelineItem* wc = static_cast<PipelineItem*>(user_data);
        wc->onBusMessage(message);
        return G_SOURCE_CONTINUE;
    }

    void setPlaying(bool playing) {
        m_playing = playing;
        if (!m_pipeline)
            return;
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), playing ? GST_STATE_PLAYING : GST_STATE_PAUSED);
        Q_EMIT playingChanged(playing);
    }

    bool playing() const {
        return m_pipeline && pipelineCurrentState(m_pipeline) == GST_STATE_PLAYING;
    }

Q_SIGNALS:
    void playingChanged(bool playing);
    void surfaceChanged();
    void descriptionChanged();
    void failed();

private:
    bool m_playing = false;
    bool m_complete = false;
    QString m_description;
    QScopedPointer<GstPipeline, GstPointerCleanup<GstPipeline>> m_pipeline;
    QGst::Quick::VideoSurface * const m_surface;
};

WebcamControl::WebcamControl()
{
    gst_init(NULL, NULL);

    QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
    engine->rootContext()->setContextObject(new KLocalizedContext(engine));

    qmlRegisterUncreatableType<Device>("org.kde.kamoso", 3, 0, "Device", "You're not supposed to mess with this yo");
    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");
    qmlRegisterType<PipelineItem>("org.kde.kamoso", 3, 0, "PipelineItem");
    qmlRegisterType<QGst::Quick::VideoItem>("KamosoQtGStreamer", 1, 0, "VideoItem");

    qmlRegisterUncreatableType<KJob>("org.kde.kamoso", 3, 0, "KJob", "you're not supposed to do that");

    m_surface = new QGst::Quick::VideoSurface(this);
    engine->rootContext()->setContextProperty("config", Settings::self());
    engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    engine->rootContext()->setContextProperty("devicesModel", DeviceManager::self());
    engine->rootContext()->setContextProperty("webcam", new Kamoso(this));
    engine->rootContext()->setContextProperty("videoSurface1", m_surface);
    engine->load(QUrl("qrc:/qml/Main.qml"));

    g_object_set(m_surface->videoSink(), "force-aspect-ratio", true, NULL);

    connect(DeviceManager::self(), &DeviceManager::playingDeviceChanged, this, &WebcamControl::play);
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

    if(m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);
        m_pipeline.reset(nullptr);
    }
}

bool WebcamControl::play()
{
    auto dev = DeviceManager::self()->playingDevice();
    return !dev || playDevice(dev);
}

static gboolean webcamWatch(GstBus     */*bus*/, GstMessage *message, gpointer user_data)
{
    WebcamControl* wc = static_cast<WebcamControl*>(user_data);
    wc->onBusMessage(message);
    return G_SOURCE_CONTINUE;
}

bool WebcamControl::playDevice(Device *device)
{
    Q_ASSERT(device);

    //If we already have a pipeline for this device, just set it to picture mode
    if (m_pipeline && m_currentDevice == device->udi()) {
        g_object_set(m_pipeline.data(), "mode", 2, nullptr);
        g_object_set(m_pipeline.data(), "location", m_tmpVideoPath.toUtf8().constData(), nullptr);
        return true;
    }
    
    //If we are changing the device, cleanup and stop old pipeline
    if (m_pipeline && m_currentDevice != device->udi()) {
        //Should we maybe try to just change the device path instead of re-creating?
        qDebug() << "playing device" << device->path() << pipelineCurrentState(m_pipeline);
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);
    }

    m_cameraSource.reset(gst_element_factory_make("wrappercamerabinsrc", "video_balance"));
    // Another option here is to return true, therefore continuing with launching, but
    // in that case the application is mostly useless.
    if (m_cameraSource.isNull()) {
        qWarning() << "The webcam controller was unable to find or load wrappercamerabinsrc plugin;"
                   << "please make sure all required gstreamer plugins are installed.";
        return false;
    }

    {
        GError* error = nullptr;
        auto source = gst_parse_bin_from_description(QByteArray("v4l2src device=") + device->path().toUtf8(), GST_PARSE_FLAG_FATAL_ERRORS, &error);
        Q_ASSERT(!error);
        g_object_set(m_cameraSource.data(), "video-source", source, nullptr);
    }

    m_pipeline.reset(GST_PIPELINE(gst_element_factory_make("camerabin", "camerabin")));
    gst_bus_add_watch (gst_pipeline_get_bus(m_pipeline.data()), &webcamWatch, this);

    g_object_set(m_pipeline.data(), "camera-source", m_cameraSource.data(), nullptr);
    g_object_set(m_pipeline.data(), "viewfinder-sink", m_surface->videoSink(), nullptr);
    setVideoSettings();

    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_READY);
    auto caps = gst_caps_from_string("video/x-raw, framerate=(fraction){30/1, 15/1}, width=(int)640, height=(int)480, format=(string){YUY2}, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive");
    g_object_set(m_pipeline.data(), "viewfinder-caps", caps, nullptr);

    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_PLAYING);

    m_currentDevice = device->udi();
    return true;
}

void WebcamControl::onBusMessage(GstMessage* message)
{
    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS: //End of stream. We reached the end of the file.
        stop();
        break;
    case GST_MESSAGE_ERROR: //Some error occurred.
        qCritical() << debugMessage(message);
        stop();
        play();
        break;
    case GST_MESSAGE_ELEMENT:
        if (strcmp (GST_MESSAGE_SRC_NAME (message), "camerabin") == 0) {
            auto structure = gst_message_get_structure (message);
            if (gst_structure_get_name (structure) == QByteArray("image-done")) {
                const gchar *filename = gst_structure_get_string (structure, "filename");
                Q_EMIT photoTaken(QString::fromUtf8(filename));
            }
        } else {
            qDebug() << "skipping message..." << GST_MESSAGE_SRC_NAME (message);
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
    g_object_set(m_pipeline.data(), "mode", 1, nullptr);

    const QString path = url.isLocalFile() ? url.toLocalFile() : QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/kamoso_photo.jpg";
    g_object_set(m_pipeline.data(), "location", path.toUtf8().constData(), nullptr);

    g_signal_emit_by_name (m_pipeline.data(), "start-capture", 0);

    if (!url.isLocalFile()) {
        KIO::copy(QUrl::fromLocalFile(path), url);
    }
}

void WebcamControl::startRecording()
{
    QString date = QDateTime::currentDateTime().toString("ddmmyyyy_hhmmss");
    m_tmpVideoPath = QDir::tempPath() + QStringLiteral("/kamoso_%1.mkv").arg(date);

    g_object_set(m_pipeline.data(), "mode", 2, nullptr);
    g_object_set(m_pipeline.data(), "location", m_tmpVideoPath.toUtf8().constData(), nullptr);

    g_signal_emit_by_name (m_pipeline.data(), "start-capture", 0);
}

QString WebcamControl::stopRecording()
{
    g_signal_emit_by_name (m_pipeline.data(), "stop-capture", 0);
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
    const auto prevstate = pipelineCurrentState(m_pipeline);
    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

    //videoflip: use video-direction=horiz, method is deprecated, not changing now because video-direction doesn't seem to be available on gstreamer 1.8 which is still widely used
    QString filters = QStringLiteral("videoflip method=4");
    if (!m_extraFilters.isEmpty()) {
        filters.prepend(m_extraFilters + QStringLiteral(" ! "));
    }

    qDebug() << "setting filter" << filters;
    GError* error = nullptr;
    g_object_set(m_cameraSource.data(), "video-source-filter", gst_parse_bin_from_description(filters.toUtf8().constData(), true, &error), nullptr);
    Q_ASSERT(!error);

    if (prevstate != GST_STATE_NULL)
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), prevstate);
}

void WebcamControl::setVideoSettings()
{
    Device *device = DeviceManager::self()->playingDevice();
    connect(device, &Device::filtersChanged, this, &WebcamControl::setExtraFilters);

    m_extraFilters = device->filters();
    updateSourceFilter();
}

#include "webcamcontrol.moc"
