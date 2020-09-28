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

#include "QGst/Quick/VideoItem"
#include <QDir>
#include <QDebug>

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
Q_PROPERTY(QQuickItem* widget READ widget WRITE setWidget NOTIFY widgetChanged)
public:
    PipelineItem()
        : QObject()
    {}

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
        if (m_widget) {
            connect(m_widget->window(), &QQuickWindow::beforeRendering, this, &PipelineItem::initialize);
        }
    }

    void initialize() {
        disconnect(m_widget->window(), &QQuickWindow::beforeRendering, this, &PipelineItem::initialize);
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
        if (!m_description.isEmpty() && m_complete && m_widget) {
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

            auto sink = gst_bin_get_by_name(GST_BIN(m_pipeline.data()), "sink");
            Q_ASSERT(sink);
            g_object_set(sink, "widget", m_widget, nullptr);
//             qDebug() << "setting " << m_widget << " to " << sink;

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

    QQuickItem* widget() const { return m_widget; }
    void setWidget(QQuickItem* widget)
    {
        if (widget == m_widget)
            return;

        m_widget = widget;
        Q_EMIT widgetChanged(widget);
        refresh();
    }

Q_SIGNALS:
    void playingChanged(bool playing);
    void surfaceChanged();
    void descriptionChanged();
    void failed();
    void widgetChanged(QQuickItem* widget);

private:
    bool m_playing = false;
    bool m_complete = false;
    QString m_description;
    GstPointer<GstPipeline> m_pipeline;
    QQuickItem* m_widget = nullptr;
};

WebcamControl::WebcamControl(QObject* parent)
    : QObject(parent)
{
    gst_init(NULL, NULL);
    gst_element_factory_make("qmlglsink", "sink"); //Necessary to have the QML item available

    m_engine = new QQmlApplicationEngine(this);
    m_engine->rootContext()->setContextObject(new KLocalizedContext(m_engine));

    qmlRegisterUncreatableType<Device>("org.kde.kamoso", 3, 0, "Device", "You're not supposed to mess with this yo");
    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");
    qmlRegisterType<PipelineItem>("org.kde.kamoso", 3, 0, "PipelineItem");
    qmlRegisterType<QGst::Quick::VideoItem>("KamosoQtGStreamer", 1, 0, "VideoItem");

    qmlRegisterUncreatableType<KJob>("org.kde.kamoso", 3, 0, "KJob", "you're not supposed to do that");

    m_engine->rootContext()->setContextProperty("config", Settings::self());
    m_engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    m_engine->rootContext()->setContextProperty("devicesModel", DeviceManager::self());
    m_engine->rootContext()->setContextProperty("webcam", new Kamoso(this));
    m_engine->rootContext()->setContextProperty("app", this);

    //qmlglsink freaks out if it gets initialized before the QSG
    connect(m_engine, &QQmlApplicationEngine::objectCreated, this, [this] (QObject* object) {
        QQuickWindow* window = qobject_cast<QQuickWindow*>(object);
        connect(window, &QQuickWindow::beforeRendering, this, &WebcamControl::initialize);
    });
    m_engine->load(QUrl("qrc:/qml/Main.qml"));

    connect(DeviceManager::self(), &DeviceManager::playingDeviceChanged, this, &WebcamControl::play);
    connect(DeviceManager::self(), &DeviceManager::noDevices, this, &WebcamControl::stop);

    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &WebcamControl::deleteLater);
}

WebcamControl::~WebcamControl()
{
    DeviceManager::self()->save();
    Settings::self()->save();
    stop();
}

void WebcamControl::stop()
{
    qDebug() << "Stop";

    if(m_pipeline) {
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);
        m_pipeline.reset();
        m_cameraSource.reset();
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

void WebcamControl::setWidget(QObject* widget)
{
    if (widget == m_widget)
        return;

    m_widget = widget;
    Q_EMIT widgetChanged(widget);
}

bool WebcamControl::playDevice(Device *device)
{
    Q_ASSERT(device);
    qDebug() << "play device" << device << m_currentDevice << device->objectId();

    //If we already have a pipeline for this device, just set it to picture mode
    if (m_pipeline && m_currentDevice == device->objectId()) {
        g_object_set(m_pipeline.data(), "mode", 2, nullptr);
        g_object_set(m_pipeline.data(), "location", m_tmpVideoPath.toUtf8().constData(), nullptr);
        return true;
    }
    qDebug() << "play device2" << device << m_pipeline << m_currentDevice << device->objectId();

    if (m_pipeline)
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

    if (!m_cameraSource) {
        m_cameraSource.reset(gst_element_factory_make("wrappercamerabinsrc", ""));
        // Another option here is to return true, therefore continuing with launching, but
        // in that case the application is mostly useless.
        if (m_cameraSource.isNull()) {
            qWarning() << "The webcam controller was unable to find or load wrappercamerabinsrc plugin;"
                       << "please make sure all required gstreamer plugins are installed.";
            return false;
        }
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

        // Convert video from webcam and set the caps
//         GstElement* capsfilter = gst_element_factory_make("capsfilter", nullptr);
//         GstCaps* caps = gst_caps_from_string("video/x-raw, format=RGBA, framerate=30/1");
        // Add glupload and do the connection with the qmlglsink for the qml widget
        GstElement* glupload = gst_element_factory_make("glupload", nullptr);
        GstElement* colorconvert = gst_element_factory_make("glcolorconvert", nullptr);

        // Connect and set all elements
//         g_object_set(capsfilter, "caps", caps, nullptr);

        auto sink = gst_element_factory_make("qmlglsink", nullptr);
        g_object_set(sink, "widget", m_widget, nullptr);
        gst_bin_add_many(GST_BIN(m_pipeline.data()), glupload, colorconvert, sink, nullptr);
        gst_element_link_many(glupload, colorconvert, sink, nullptr);
        g_object_set(m_pipeline.data(), "viewfinder-sink", glupload, nullptr);
    }

    setVideoSettings();

    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_READY);

    gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_PLAYING);

    m_currentDevice = device->objectId();
    return true;
}

void WebcamControl::onBusMessage(GstMessage* message)
{
    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS: //End of stream. We reached the end of the file.
        stop();
        break;
    case GST_MESSAGE_ERROR: {//Some error occurred.
        static int error = 0;
        qCritical() << "error:" << debugMessage(message);
        stop();
        if (error < 3) {
//             play();
            ++error;
        }
    }   break;
    case GST_MESSAGE_ELEMENT:
        if (strcmp (GST_MESSAGE_SRC_NAME (message), "camerabin") == 0) {
            auto structure = gst_message_get_structure (message);
            if (gst_structure_get_name (structure) == QByteArray("image-done")) {
                const gchar *filename = gst_structure_get_string (structure, "filename");
                if (m_emitTaken)
                    Q_EMIT photoTaken(QString::fromUtf8(filename));
            }
        } else {
            qDebug() << "skipping message..." << GST_MESSAGE_SRC_NAME (message);
        }
    default:
//         qDebug() << message->type;
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

    const QString path = url.isLocalFile() ? url.toLocalFile() : QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/kamoso_photo.jpg";
    g_object_set(m_pipeline.data(), "location", path.toUtf8().constData(), nullptr);

    g_signal_emit_by_name (m_pipeline.data(), "start-capture", 0);

    if (!url.isLocalFile()) {
        KIO::copy(QUrl::fromLocalFile(path), url);
    }

    if (emitTaken) {
        KNotification::event(QStringLiteral("photoTaken"), i18n("Photo taken"), i18n("Saved in %1", url.toDisplayString(QUrl::PreferLocalFile)));
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

void WebcamControl::initialize()
{
//     if (!play()) {
//         qWarning("Unrecoverable error occurred when initializing webcam. Exiting.");
//         QCoreApplication::instance()->exit(1);
//     }
//
//     disconnect(qobject_cast<QQuickWindow*>(m_engine->rootObjects().constFirst()), &QQuickWindow::beforeRendering, this, &WebcamControl::initialize);
}

void WebcamControl::updateSourceFilter()
{
    if (!m_pipeline)
        return;

    const auto prevstate = pipelineCurrentState(m_pipeline);
    if (prevstate != GST_STATE_NULL)
        gst_element_set_state(GST_ELEMENT(m_pipeline.data()), GST_STATE_NULL);

    QString filters = m_mirror ? QStringLiteral("videoflip video-direction=horiz") : QStringLiteral("videoflip method=none");
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

#include "webcamcontrol.moc"
