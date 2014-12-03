/*************************************************************************************
 *  Copyright (C) 2012 by Aleix Pol <aleixpol@kde.org>                               *
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

#ifndef KAMOSODIRMODEL_H
#define KAMOSODIRMODEL_H

#include <kdirmodel.h>

class KamosoDirModel : public KDirModel
{
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList mimeFilter READ mimeFilter WRITE setMimeFilter NOTIFY filterChanged)
    Q_OBJECT
    public:
        enum Roles {
            Path = ColumnCount+1,
            MimeType
        };

        explicit KamosoDirModel(QObject* parent = 0);
        void setUrl(const QUrl& url);
        QUrl url() const;

        void setMimeFilter(const QStringList& mimes);
        QStringList mimeFilter() const;

        virtual QHash<int, QByteArray> roleNames() const;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_SIGNALS:
        void urlChanged();
        void filterChanged();
};

#endif // KAMOSODIRMODEL_H
