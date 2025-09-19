/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>
    SPDX-FileCopyrightText: 2013 Michael G. Hansen <mike@mghansen.de>
    SPDX-FileCopyrightText: 2022 Dmitrii Fomchenkov <fomchenkovda@basealt.ru>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "kamoso.h"

#include "kamosoSettings.h"
#include "video/webcamcontrol.h"
#include "devicemanager.h"
#include <KIO/Global>
#include <KIO/CopyJob>
#include <KIO/MkpathJob>
#include <KIO/FileUndoManager>
#include <KIO/JobUiDelegate>
#include <KFormat>
#include <KFileUtils>
#include <KLocalizedString>
#include <QFile>
#include <QJsonArray>
#include <QDir>
#include <QTemporaryFile>
#include <QIcon>

Kamoso::Kamoso(WebcamControl *webcamControl)
    : m_webcamControl(webcamControl)
{
    m_recordingTime.restart();
    m_recordingTimer.setInterval(30);

    connect(m_webcamControl, &WebcamControl::mirroredChanged, this, &Kamoso::mirroredChanged);
    connect(m_webcamControl, &WebcamControl::photoTaken, this, &Kamoso::photoTaken);
    connect(&m_recordingTimer, &QTimer::timeout, this, &Kamoso::recordingTimeChanged);
}

Kamoso::~Kamoso() = default;

QString Kamoso::fileNameSuggestion(const QString &destination, const QString &name, const QString& extension) const
{
    const QString date = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd_hh-mm-ss"));
    const QString initialName =  QStringLiteral("%1_%2.%3").arg(name, date, extension);

    QString suggestion (destination + u'/' + initialName);

    if (QFile f(suggestion); f.exists()) {
        suggestion = destination + u'/' + KFileUtils::suggestName(QUrl::fromLocalFile(destination), initialName);
    }

    return suggestion;
}

const QString Kamoso::takePhoto()
{
    const auto savePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    const QString path = fileNameSuggestion(savePath, QStringLiteral("picture"), QStringLiteral("jpg"));
    m_webcamControl->takePhoto(path, true);

    if (m_sampleImagePath.isEmpty()) {
        m_sampleImagePath = path;
        Q_EMIT sampleImageChanged(m_sampleImagePath);
    }

    return path;
}

void Kamoso::resetDeviceSettings()
{
    Device *device = DeviceManager::self()->playingDevice();
    device->reset();
}

void Kamoso::setRecording(bool recording)
{
    if (!DeviceManager::self()->playingDevice()) {
        qWarning() << "No device found";
        Q_EMIT error(i18n("No device found"));
        return;
    }

    if (recording == m_recordingTimer.isActive())
        return;

    if (recording) {
        const auto savePath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        m_webcamControl->startRecording(fileNameSuggestion(savePath, QStringLiteral("video"), QStringLiteral("mkv")));
        m_recordingTime.restart();
        m_recordingTimer.start();
    } else {
        m_webcamControl->stopRecording();
        m_webcamControl->playDevice(DeviceManager::self()->playingDevice());
        m_recordingTimer.stop();
    }

    Q_EMIT isRecordingChanged(m_recordingTimer.isActive());
}

QString Kamoso::recordingTime() const
{
    return KFormat().formatDuration(m_recordingTime.elapsed());
}

bool Kamoso::isRecording() const
{
    return m_recordingTimer.isActive();
}

void Kamoso::trashFiles(const QJsonArray& urls)
{
    QList<QUrl> list;
    for(const QJsonValue& val : urls) {
        list += QUrl(val.toString());
    }

    KIO::Job* job = KIO::trash(list);
    KIO::FileUndoManager::self()->recordJob(KIO::FileUndoManager::Trash, list, QUrl(QStringLiteral("trash:/")), job);
}

void Kamoso::setSampleImage(const QString &path)
{
    if (path == m_sampleImagePath)
        return;
    m_sampleImagePath = path;
    Q_EMIT sampleImageChanged(path);
}

QString Kamoso::sampleImage() const
{
    if (!m_sampleRequested && m_sampleImagePath.isEmpty()) {
        m_sampleRequested = true;
        auto temporaryFile = std::make_unique<QTemporaryFile>(QDir::temp().absoluteFilePath(QStringLiteral("XXXXXX-sampleimage.jpg")));
        temporaryFile->open();
        const auto path = temporaryFile->fileName();
        temporaryFile->close();

        m_webcamControl->takePhoto(path, false);
    }
    return m_sampleImagePath;
}

bool Kamoso::mirrored() const
{
    return m_webcamControl->mirrored();
}

void Kamoso::setMirrored(bool m)
{
    m_webcamControl->setMirrored(m);
}
