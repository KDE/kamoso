/*
    SPDX-FileCopyrightText: 2012 Alejandro Fiestas Olivares <afiestaso@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef WEBCAMCONTROL_H
#define WEBCAMCONTROL_H

#include <QObject>
#include <QUrl>

#include "gstpointer.h"
#include <gst/gstpipeline.h>
#include <gst/gstmessage.h>
#include <kamoso.h>

namespace QGst { namespace Quick { class VideoSurface; } }

class Device;
class WebcamControl : public QObject
{
    Q_OBJECT
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

        bool mirrored() const { return m_mirror; }

    public Q_SLOTS:
        bool play();
        bool playDevice(Device* device);
        void stop();
        void takePhoto(const QUrl& url, bool emitTaken);
        void startRecording();
        QString stopRecording();

    private Q_SLOTS:
        void setExtraFilters(const QString &extraFilters);

    Q_SIGNALS:
        void photoTaken(const QString &photoUrl);
        void mirroredChanged(bool mirrored);

    private:
        void updateSourceFilter();
        void setVideoSettings();

        QString m_extraFilters;
        QString m_tmpVideoPath;
        QString m_currentDevice;
        GstPointer<GstPipeline> m_pipeline;
        GstPointer<GstElement> m_cameraSource;
        Kamoso* m_kamoso;
        QGst::Quick::VideoSurface* m_surface = nullptr;
        bool m_emitTaken = true;
        bool m_mirror = true;
};

#endif // WEBCAMCONTROL_H
