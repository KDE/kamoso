/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include <QWidget>
#include <QGst/Ui/VideoWidget>

class KJob;
class Device;
class KUrl;
class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

class WebcamWidget : public QGst::Ui::VideoWidget
{
Q_OBJECT
public:
    static WebcamWidget* createInstance(QWidget *parent);
    static WebcamWidget* self();
    ~WebcamWidget();

    void photoGstCallback(QGst::BufferPtr buffer, QGst::PadPtr pad);
    virtual QSize sizeHint() const;
public Q_SLOTS:
    void playFile(const Device& device);
    bool takePhoto(const KUrl &dest);
    void recordVideo(bool sound);
    void stopRecording(const KUrl& destUrl);
    void fileSaved(KJob *);
    void setBrightness(int level);
    void setSaturation(int level);
    void setContrast(int level);
    void setGamma(int level);
    void setHue(int level);
    void setVideoSettings();

Q_SIGNALS:
    void fileSaved(const KUrl &url);

private:
    WebcamWidget(QWidget* parent);
    QByteArray phononCaptureDevice();
    QByteArray basicPipe();
    void setDevice(const Device &device);
    float convertAdjustValue(int level);
    void activeAspectRatio();
    void onBusMessage(const QGst::MessagePtr &message);

private:
    static WebcamWidget* s_instance;
    class Private;
    Private* d;

};

#endif //WEBCAMWIDGET
