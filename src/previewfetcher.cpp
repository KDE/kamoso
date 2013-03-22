/*************************************************************************************
 *  Copyright (C) 2013 by Aleix Pol <aleixpol@kde.org>                               *
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

#include "previewfetcher.h"
#include <kio/previewjob.h>
#include <KIcon>

PreviewFetcher::PreviewFetcher(QObject* parent)
    : QObject(parent)
    , m_size(-1,-1)
{
}

void PreviewFetcher::setUrl(const QUrl& url)
{
    m_url = url;
    emit urlChanged();
    fetchPreview();
}

QUrl PreviewFetcher::url() const
{
    return m_url;
}

void PreviewFetcher::fetchPreview()
{
    if(m_size.isEmpty() || !m_size.isValid() || m_url.isEmpty()) {
        m_preview = QPixmap();
        emit previewChanged();
        return;
    }
    KIO::PreviewJob* job = new KIO::PreviewJob(KFileItemList() << KFileItem(KUrl(m_url), m_mimetype, 0), m_size);
    connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(updatePreview(KFileItem,QPixmap)));
    connect(job, SIGNAL(failed(KFileItem)), SLOT(fallbackPreview(KFileItem)));
    job->start();
}

void PreviewFetcher::setHeight(int h)
{
    m_size.setHeight(h);
    fetchPreview();
}

void PreviewFetcher::setWidth(int w)
{
    m_size.setWidth(w);
    fetchPreview();
}

void PreviewFetcher::updatePreview(const KFileItem& changed, const QPixmap& prev)
{
    m_preview = prev;
    emit previewChanged();
}

QString PreviewFetcher::mimeType() const
{
    return m_mimetype;
}

void PreviewFetcher::setMimeType(const QString& mime)
{
    m_mimetype = mime;
    fetchPreview();
}

QPixmap PreviewFetcher::preview() const
{
    return m_preview;
}

void PreviewFetcher::fallbackPreview(const KFileItem& item)
{
    m_preview = KIcon(item.mimeTypePtr()->iconName()).pixmap(m_size);
    emit previewChanged();
}
