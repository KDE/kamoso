/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>
    SPDX-FileCopyrightText: 2013 Michael G. Hansen <mike@mghansen.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef KAMOSO_H
#define KAMOSO_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QUrl>
#include <QTemporaryFile>

class WebcamControl;
class QWindow;

class Kamoso : public QObject
{
Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording WRITE setRecording NOTIFY isRecordingChanged)
    Q_PROPERTY(QString recordingTime READ recordingTime NOTIFY recordingTimeChanged)
    Q_PROPERTY(QString sampleImage READ sampleImage NOTIFY sampleImageChanged)
    Q_PROPERTY(bool mirrored READ mirrored WRITE setMirrored NOTIFY mirroredChanged)

    public:
        explicit Kamoso(WebcamControl* webcamControl);
        virtual ~Kamoso();

        bool isRecording() const;
        QString recordingTime() const;
        void setRecording(bool recording);

        Q_SCRIPTABLE void trashFiles(const QJsonArray& urls);

        void setSampleImage(const QString &path);
        QString sampleImage() const;

        void setMirrored(bool m);
        bool mirrored() const;

    public Q_SLOTS:
        const QString takePhoto();
        void resetDeviceSettings();

    Q_SIGNALS:
        void photoTaken(const QString &path);
        void isRecordingChanged(bool isRecording);
        void recordingTimeChanged();
        void sampleImageChanged(const QString &sampleImage);
        void mirroredChanged(bool mirrored);
        void error(const QString &error);

    private:
        QString fileNameSuggestion(const QString &destinationPath, const QString &name, const QString& extension) const;

        WebcamControl * const m_webcamControl;
        QTimer m_recordingTimer;
        QElapsedTimer m_recordingTime;
        QString m_sampleImagePath;
        mutable bool m_sampleRequested = false;
};

#endif // KAMOSO_H
