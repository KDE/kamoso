/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <afiestas@kde.org>                       *
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

#include "youtubejobcomposite.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QStandardPaths>

YoutubeJobComposite::YoutubeJobComposite()
    : ShareJob()
{
}

void YoutubeJobComposite::start()
{
    foreach(const QJsonValue& url, data().value("urls").toArray()) {
//         qDebug() << "Url to upload: " << url;
        YoutubeJob* job = new YoutubeJob(QUrl(url.toString()),
                                         data().value("videoTitle").toString(),
                                         data().value("videoTags").toString(),
                                         data().value("videoDesc").toString(), this);
        connect(job, &KJob::finished, this, &YoutubeJobComposite::subjobFinished);
        m_pendingJobs++;
    }
}

void YoutubeJobComposite::subjobFinished(KJob* subjob)
{
    m_pendingJobs--;
    if (subjob->error()) {
        setError(subjob->error());
        setErrorText(subjob->errorText());
    }
    if (m_pendingJobs==0) {
        emitResult();
    }
}

QUrl YoutubeJobComposite::configSourceCode() const
{
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kamoso/share/youtubeplugin_config.qml");
    Q_ASSERT(!path.isEmpty());
    return QUrl::fromLocalFile(path);
}
