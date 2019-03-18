/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
 *  Copyright (C) 2013      by Michael G. Hansen <mike@mghansen.de>                  *
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


#ifndef KAMOSO_H
#define KAMOSO_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QUrl>

class WebcamControl;
class QWindow;

class Kamoso : public QObject
{
Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording WRITE setRecording NOTIFY isRecordingChanged)
    Q_PROPERTY(QString recordingTime READ recordingTime NOTIFY recordingTimeChanged)
    Q_PROPERTY(QString sampleImage READ sampleImage NOTIFY sampleImageChanged)

    public:
        explicit Kamoso(WebcamControl* webcamControl);
        virtual ~Kamoso();

        bool isRecording() const;
        QString recordingTime() const;
        void setRecording(bool recording);

        Q_SCRIPTABLE void trashFiles(const QJsonArray& urls);
        QString sampleImage();

    public Q_SLOTS:
        const QString takePhoto();
        void resetDeviceSettings();

    Q_SIGNALS:
        void photoTaken(const QString &path);
        void isRecordingChanged(bool isRecording);
        void recordingTimeChanged();
        void sampleImageChanged(const QString &sampleImage);

    private:
        QUrl fileNameSuggestion(const QUrl &saveUrl, const QString &name, const QString& extension) const;

        WebcamControl * const m_webcamControl;
        QTimer m_recordingTimer;
        QElapsedTimer m_recordingTime;
        QString m_sampleImagePath;
};

#endif // KAMOSO_H
