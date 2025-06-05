/*
    SPDX-FileCopyrightText: 2012 Alejandro Fiestas Olivares <afiestaso@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef WEBCAMCONTROL_H
#define WEBCAMCONTROL_H

#include <QObject>
#include <QUrl>
#include <QQuickItem>

#include "gstpointer.h"
#include <gst/gstpipeline.h>
#include <gst/gstmessage.h>
#include <kamoso.h>

namespace QGst { namespace Quick { class VideoSurface; } }

class Device;
class WebcamControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentDevice READ currentDevice NOTIFY currentDeviceChanged)
    Q_PROPERTY(bool readyForCapture READ readyForCapture NOTIFY readyForCaptureChanged)
    public:
        WebcamControl();
        virtual ~WebcamControl();

        void onBusMessage(GstMessage* msg);
        void setMirrored(bool m) {
            if (m != m_mirror) {
                m_mirror = m;
                updateSourceFilter();
                Q_EMIT mirroredChanged(m);
            }
        }

        QString currentDevice() const { return m_currentDevice; }
        bool mirrored() const { return m_mirror; }
        bool readyForCapture() const { return m_readyForCapture; }

    public Q_SLOTS:
        bool play();
        bool playDevice(Device* device);
        void stop();
        void takePhoto(const QUrl& url, bool emitTaken);
        void startRecording(const QUrl &destination);
        void stopRecording();

    private Q_SLOTS:
        void setExtraFilters(const QString &extraFilters);

    Q_SIGNALS:
        void photoTaken(const QString &photoUrl);
        void mirroredChanged(bool mirrored);
        void currentDeviceChanged();
        void readyForCaptureChanged();

    private:
        static void captureReadyChanged(GObject *object, GParamSpec *pspec, gpointer user_data);

        void setReadyForCapture(bool b);
        void updateSourceFilter();
        void setVideoSettings();

        QString m_extraFilters;
        QString m_tmpVideoPath;
        QUrl m_videoDestination;
        QString m_currentDevice;
        GstPointer<GstPipeline> m_pipeline;
        GstPointer<GstElement> m_cameraSource;
        GstPointer<GstElement> m_viewfinder;
        Kamoso* m_kamoso;
        QQuickItem* m_surface = nullptr;
        bool m_emitTaken = true;
        bool m_mirror = true;
        bool m_readyForCapture = true;
};

#endif // WEBCAMCONTROL_H
