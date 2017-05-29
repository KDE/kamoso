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

#ifndef PREVIEWFETCHER_H
#define PREVIEWFETCHER_H

#include <QObject>
#include <QUrl>
#include <QPixmap>

class KFileItem;
class PreviewFetcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QPixmap preview READ preview NOTIFY previewChanged)
    Q_PROPERTY(QString mimetype READ mimeType WRITE setMimeType)
    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(QSize previewSize READ previewSize NOTIFY previewChanged)
    public:
        explicit PreviewFetcher(QObject* parent = 0);

        QUrl url() const;
        void setUrl(const QUrl& url);

        QSize previewSize() const { return m_preview.size(); }
        QSize size() const { return m_size; }
        void setSize(const QSize &size);

        QPixmap preview() const;

        QString mimeType() const;
        void setMimeType(const QString& mime);
    Q_SIGNALS:
        void urlChanged();
        void previewChanged();

    private Q_SLOTS:
        void fetchPreview();
        void updatePreview(const KFileItem&, const QPixmap& p);
        void fallbackPreview(const KFileItem& item);

    private:
        void setPreview(const QPixmap& preview);

        QSize m_size;
        QUrl m_url;
        QPixmap m_preview;
        QString m_mimetype;
};

#endif // PREVIEWFETCHER_H
