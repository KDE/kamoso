/*
    SPDX-FileCopyrightText: 2012 Aleix Pol <aleixpol@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KAMOSODIRMODEL_H
#define KAMOSODIRMODEL_H

#include <KDirModel>
#include <KDirSortFilterProxyModel>

class KamosoDirModel : public KDirSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList mimeFilter READ mimeFilter WRITE setMimeFilter NOTIFY filterChanged)
    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter)
    public:
        enum Roles {
            Path = KDirModel::ColumnCount+1,
            MimeType
        };

        explicit KamosoDirModel(QObject* parent = nullptr);
        void setUrl(const QUrl& url);
        QUrl url() const;

        QString nameFilter() const;
        void setNameFilter(const QString& filter);

        void setMimeFilter(const QStringList& mimes);
        QStringList mimeFilter() const;

        QHash<int, QByteArray> roleNames() const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    Q_SIGNALS:
        void urlChanged();
        void filterChanged();

    private:
        KDirModel* m_dirModel;
};

#endif // KAMOSODIRMODEL_H
