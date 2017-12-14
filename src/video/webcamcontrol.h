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

#include <gst/gstpipeline.h>
#include <gst/gstmessage.h>

namespace QGst { namespace Quick { class VideoSurface; } }

template <typename T>
struct GstPointerCleanup
{
    static inline void cleanup(T *pointer)
    {
        if (pointer) gst_object_unref (pointer);
    }
};

class Device;
class WebcamControl : public QObject
{
    Q_OBJECT
    public:
        WebcamControl();
        virtual ~WebcamControl();

        void onBusMessage(GstMessage* msg);

    public Q_SLOTS:
        bool play();
        bool playDevice(Device* device);
        void stop();
        void takePhoto(const QUrl& url);
        void startRecording();
        QString stopRecording();

    private Q_SLOTS:
        void setExtraFilters(const QString &extraFilters);

    Q_SIGNALS:
        void photoTaken(const QString &photoUrl);

    private:
        void updateSourceFilter();
        void setVideoSettings();

        QString m_extraFilters;
        QString m_tmpVideoPath;
        QString m_currentDevice;
        QScopedPointer<GstPipeline, GstPointerCleanup<GstPipeline> > m_pipeline;
        QScopedPointer<GstElement, GstPointerCleanup<GstElement> > m_cameraSource;
        QGst::Quick::VideoSurface* m_surface = nullptr;
};

#endif // WEBCAMCONTROL_H
