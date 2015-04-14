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

#include <QtCore/QObject>

#include <QQuickView>
#include <QGst/Pipeline>
#include <QUrl>

class Device;
class WebcamControl : public QObject
{
    Q_OBJECT
    public:
        WebcamControl();
        virtual ~WebcamControl();

        void onBusMessage(const QGst::MessagePtr & msg);
    public Q_SLOTS:
        void play();
        void play(Device* device);
        void stop();
        void takePhoto(const QUrl& url);
        void startRecording();
        QString stopRecording();

    private Q_SLOTS:
        void setBrightness(int level);
        void setContrast(int level);
        void setSaturation(int level);
        void setGamma(int level);
        void setHue(int level);

    Q_SIGNALS:
        void photoTaken(const QString &photoUrl);

    private:
        void setVideoSettings();

        QString m_tmpVideoPath;
        QString m_currentDevice;
        QGst::PipelinePtr m_pipeline;
        QGst::ElementPtr m_videoSink;
        QGst::ElementPtr m_videoBalance;
        QGst::ElementPtr m_gamma;
};

#endif // WEBCAMCONTROL_H
