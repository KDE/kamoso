/*
    SPDX-FileCopyrightText: 2012 Alejandro Fiestas Olivares <afiestaso@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "webcamcontrol.h"
#include "kamosoSettings.h"
#include <devicemanager.h>
#include <kamosodirmodel.h>
#include <previewfetcher.h>
#include <kamoso.h>
#include <KIO/CopyJob>
#include <KNotification>
#include <KLocalizedString>

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

#include <QDir>
#include <QDebug>
#include <QRunnable>
#include <QThread>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickWindow>
#include <qqml.h>
#include <KJob>
#include <KLocalizedContext>

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
        // qWarning() << "error debugMessage:" << e->message;
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
    if(result == GST_STATE_CHANGE_FAILURE)
        qDebug() << "broken state";
    return currentState;
}

class PipelineItem : public QObject, public QQmlParserStatus
{
Q_OBJECT
Q_INTERFACES(QQmlParserStatus)
Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
public:
    PipelineItem()
        : QObject()
    {
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
            Q_EMIT failed();
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

    QString description() const { return m_description; }
    void setDescription(const QString &desc) {
        if (m_description != desc) {
            m_description = desc;
            Q_EMIT descriptionChanged();

            refresh();
        }
    }

    Q_SCRIPTABLE void refresh() {
        if (!m_description.isEmpty() && m_complete && parent()) {
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

            auto lastItem = gst_bin_get_by_name(GST_BIN(m_pipeline.data()), "last");
            Q_ASSERT(lastItem);

            auto glupload = gst_element_factory_make_full("glupload", nullptr);
            auto sink = gst_element_factory_make_full("qml6glsink", "name", "videosink"
                                                                          , "widget", qobject_cast<QQuickItem *>(parent())
                                                                          , "async", true
                                                                          , nullptr);
            gst_bin_add_many(GST_BIN(m_pipeline.data()), glupload, sink, nullptr);
            bool correct = gst_element_link_many(lastItem, glupload, sink, nullptr);
            Q_ASSERT(correct);
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
        if (playing != m_playing) {
            m_playing = playing;
            Q_EMIT playingChanged(playing);
        }

        if (m_pipeline)
            gst_element_set_state(GST_ELEMENT(m_pipeline.data()), playing ? GST_STATE_PLAYING : GST_STATE_PAUSED);
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
    GstPointer<GstPipeline> m_pipeline;
    GstPointer<GstElement> m_viewfinder;
};

class PlayWebcam : public QRunnable
{
public:
    PlayWebcam(WebcamControl *wc)
        : m_wc(wc)
    {}

    void run() override {
        m_wc->play();
    }

private:
    WebcamControl *const m_wc;
};

WebcamControl::WebcamControl()
{
    QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
    engine->rootContext()->setContextObject(new KLocalizedContext(engine));
    m_kamoso = new Kamoso(this);

    qmlRegisterUncreatableType<Device>("org.kde.kamoso", 3, 0, "Device", QStringLiteral("You're not supposed to mess with this yo"));
    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");
    qmlRegisterType<PipelineItem>("org.kde.kamoso", 3, 0, "PipelineItem");
    qmlRegisterSingletonInstance<WebcamControl>("org.kde.kamoso", 3, 0, "WebcamControl", this);
    qmlRegisterSingletonInstance<Kamoso>("org.kde.kamoso", 3, 0, "Kamoso", m_kamoso);
    qmlRegisterSingletonInstance<DeviceManager>("org.kde.kamoso", 3, 0, "DeviceManager", DeviceManager::self());
    qmlRegisterSingletonInstance<Settings>("org.kde.kamoso", 3, 0, "Settings", Settings::self());

    qmlRegisterUncreatableType<KJob>("org.kde.kamoso", 3, 0, "KJob", QStringLiteral("you're not supposed to do that"));

    engine->load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    auto rootObject = qobject_cast<QQuickWindow *> (engine->rootObjects().constFirst());
    m_surface = rootObject->findChild<QQuickItem *>("sink");
    Q_ASSERT(m_surface);

    connect(DeviceManager::self(), &DeviceManager::playingDeviceChanged, this, &WebcamControl::play);
    connect(DeviceManager::self(), &DeviceManager::noDevices, this, &WebcamControl::stop);

    // If we call play too early, qml6glsink freaks out
    rootObject->scheduleRenderJob(new PlayWebcam(this), QQuickWindow::BeforeSynchronizingStage);
}

WebcamControl::~WebcamControl()
{
    DeviceManager::self()->save();
    Settings::self()->save();
    delete m_kamoso;
}

void WebcamControl::stop()
{
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

void WebcamControl::captureReadyChanged(GObject *camera_bin, GParamSpec *pspec, gpointer user_data)
{
    gboolean is_ready;
    g_object_get(camera_bin, "ready-for-capture", &is_ready, nullptr);

    WebcamControl* wc = static_cast<WebcamControl*>(user_data);
    wc->setReadyForCapture(is_ready);
}

bool WebcamControl::playDevice(Device *device)
{
    Q_ASSERT(device);

    //If we already have a pipeline for this device, just set it to picture mode
    if (m_pipeline && m_currentDevice == device->objectId()) {
        g_object_set(m_pipeline.data(), "mode", 2, nullptr);
        g_object_set(m_pipeline.data(), "location", m_tmpVideoPath.toUtf8().constData(), nullptr);
        return true;
    }

    if (m_pipeline)
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

    if (!m_cameraSource) {
        m_cameraSource.reset(gst_element_factory_make("wrappercamerabinsrc", ""));
        g_object_set(m_cameraSource.data(), "async-handling", true, nullptr);
        g_signal_connect(m_cameraSource.get(), "notify::ready-for-capture",
                         G_CALLBACK(captureReadyChanged), this);

        // Another option here is to return true, therefore continuing with launching, but
        // in that case the application is mostly useless.
        if (m_cameraSource.isNull()) {
            qWarning() << "The webcam controller was unable to find or load wrappercamerabinsrc plugin;"
                       << "please make sure all required gstreamer plugins are installed.";
            return false;
        }
    }
    if (!m_viewfinder) {
        Q_ASSERT(m_surface);
        m_viewfinder.reset(gst_bin_new("viewfinder"));
        auto glupload = gst_element_factory_make_full("glupload", nullptr);
        auto sink = gst_element_factory_make_full("qml6glsink", "name", "videosink"
                                                                    , "widget", m_surface
                                                                    , "async", true
                                                                    , nullptr);
        gst_bin_add_many(GST_BIN(m_viewfinder.data()), glupload, sink, nullptr);
        gst_element_link(glupload, sink);

        // Add ghost pad to expose sink pad of glupload as the bin's pad
        GstPad *sink_pad = gst_element_get_static_pad(glupload, "sink");
        gst_element_add_pad(m_viewfinder.data(), gst_ghost_pad_new("sink", sink_pad));
        gst_object_unref(sink_pad);
    }

    GstElement* source;
    g_object_get(m_cameraSource.data(), "video-source", &source, nullptr);
    if (m_currentDevice != device->objectId() || !source) {
        source = device->createElement();
        g_object_set(m_cameraSource.data(), "video-source", source, nullptr);
    }

    if (!m_pipeline) {
        m_pipeline.reset(GST_PIPELINE(gst_element_factory_make("camerabin", "camerabin")));
        gst_bus_add_watch (gst_pipeline_get_bus(m_pipeline.data()), &webcamWatch, this);
        g_object_set(m_pipeline.data(), "camera-source", m_cameraSource.data(), nullptr);
        g_object_set(m_pipeline.data(), "viewfinder-sink", m_viewfinder.data(), nullptr);
    }

    setVideoSettings();

    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_PLAYING);

    m_currentDevice = device->objectId();
    Q_EMIT currentDeviceChanged();
    return true;
}

void WebcamControl::onBusMessage(GstMessage* message)
{
    // qDebug() << "bus message" << GST_MESSAGE_SRC_NAME (message) << gst_structure_to_string(gst_message_get_structure (message));
    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS: //End of stream. We reached the end of the file.
        stop();
        break;
    case GST_MESSAGE_ERROR: {//Some error occurred.
        static int error = 0;
        qCritical() << "error:" << debugMessage(message);
        stop();
        if (error < 3) {
            play();
            ++error;
        }
    }   break;
    case GST_MESSAGE_ELEMENT:
        if (strcmp (GST_MESSAGE_SRC_NAME (message), "camerabin") == 0) {
            auto structure = gst_message_get_structure (message);
            if (gst_structure_get_name (structure) == QByteArray("image-done")) {
                const gchar *filename = gst_structure_get_string (structure, "filename");
                const QString file = QString::fromUtf8(filename);
                if (m_emitTaken)
                    Q_EMIT photoTaken(file);
                else {
                    m_kamoso->setSampleImage(file);
                }
            } else if (gst_structure_get_name(structure) ==
                       QByteArray("video-done")) {
              g_object_set(m_pipeline.data(), "mode", 1, nullptr);
              if (!m_videoDestination.isLocalFile()) {
                KJob *job = KIO::move(QUrl::fromLocalFile(m_tmpVideoPath),
                                      m_videoDestination);
                job->start();
                connect(job, &KJob::finished, this, [this, job] {
                  if (job->error() == 0) {
                    qDebug() << "video saved successfully";
                    return;
                  }

                  qWarning() << "Could not move" << m_tmpVideoPath << "to"
                             << m_videoDestination;
                  Q_EMIT m_kamoso->error(job->errorString());
                });
              }
            } else {
              qDebug() << "Ignoring message from camerabin"
                       << gst_structure_get_name(structure);
            }
        }
        break;
    default:
        break;
    }
}
void WebcamControl::takePhoto(const QUrl &url, bool emitTaken)
{
    if (!m_pipeline) {
        qWarning() << "couldn't take photo, no pipeline";
        return;
    }
    m_emitTaken = emitTaken;

    g_object_set(m_pipeline.data(), "mode", 1, nullptr);

    const QString path = url.isLocalFile() ? url.toLocalFile() : QStandardPaths::writableLocation(QStandardPaths::TempLocation) + u"/kamoso_photo.jpg";
    g_object_set(m_pipeline.data(), "location", path.toUtf8().constData(), nullptr);

    g_signal_emit_by_name (m_pipeline.data(), "start-capture", 0);

    if (!url.isLocalFile()) {
        KIO::copy(QUrl::fromLocalFile(path), url);
    }

    if (emitTaken) {
        KNotification::event(QStringLiteral("photoTaken"), i18n("Photo taken"), i18n("Saved in %1", url.toDisplayString(QUrl::PreferLocalFile)));
    }
}

static QString temporaryVideoFile()
{
    QString date = QDateTime::currentDateTime().toString(u"ddmmyyyy_hhmmss");
    return QDir::tempPath() + QStringLiteral("/kamoso_%1.mkv").arg(date);
}

void WebcamControl::startRecording(const QUrl &destination)
{
    m_videoDestination = destination;
    m_tmpVideoPath = destination.isLocalFile() ? destination.toLocalFile() : temporaryVideoFile();

    g_object_set(m_pipeline.data(), "mode", 2,
                                    "location", m_tmpVideoPath.toUtf8().constData(),
                                    nullptr);

    g_signal_emit_by_name (m_pipeline.data(), "start-capture", 0);
}

void WebcamControl::stopRecording()
{
    g_signal_emit_by_name(m_pipeline.data(), "stop-capture", 0);
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
    if (!m_pipeline)
        return;

    const auto prevstate = pipelineCurrentState(m_pipeline);
    if (prevstate != GST_STATE_NULL)
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

    QString filters = QStringLiteral("videoflip video-direction=auto");
    if (m_mirror)
        filters += QStringLiteral(" ! videoflip video-direction=horiz");

    if (!m_extraFilters.isEmpty()) {
        if (!filters.isEmpty())
            filters.prepend(QStringLiteral(" ! "));
        filters.prepend(m_extraFilters);
    }

    if (!filters.isEmpty()) {
        GError* error = nullptr;
        auto elem = gst_parse_bin_from_description(filters.toUtf8().constData(), true, &error);
        if (error) {
            qDebug() << "error" << error->message;
        }
        Q_ASSERT(!error);
        g_object_set(m_cameraSource.data(), "video-source-filter", elem, nullptr);
    } else {
        g_object_set(m_cameraSource.data(), "video-source-filter", nullptr, nullptr);
    }

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

void WebcamControl::setReadyForCapture(bool readyForCapture)
{
    if (m_readyForCapture == readyForCapture) {
        return;
    }
    qDebug() << "camera ready:" << readyForCapture;
    m_readyForCapture = readyForCapture;
    Q_EMIT readyForCaptureChanged();
}

#include "webcamcontrol.moc"
