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


#ifndef WEBCAMCONTROL_H
#define WEBCAMCONTROL_H

#include <QObject>
#include <QUrl>

#include "gstpointer.h"
#include <gst/gstpipeline.h>
#include <gst/gstmessage.h>

namespace QGst { namespace Quick { class VideoSurface; } }

class Device;
class QQmlApplicationEngine;

class WebcamControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* widget READ widget WRITE setWidget NOTIFY widgetChanged)
    public:
        WebcamControl(QObject* parent);
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

        QObject* widget() const { return m_widget; }
        void setWidget(QObject* widget);

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
        void widgetChanged(QObject* widget);

    private:
        void initialize();
        void updateSourceFilter();
        void setVideoSettings();

        QString m_extraFilters;
        QString m_tmpVideoPath;
        QString m_currentDevice;
        GstPointer<GstPipeline> m_pipeline;
        GstPointer<GstElement> m_cameraSource;
        bool m_emitTaken = true;
        bool m_mirror = true;
        QObject* m_widget = nullptr;
        QQmlApplicationEngine* m_engine;
};

#endif // WEBCAMCONTROL_H
