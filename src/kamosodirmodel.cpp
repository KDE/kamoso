/*
    SPDX-FileCopyrightText: 2012 Aleix Pol <aleixpol@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kamosodirmodel.h"
#include <kdirlister.h>

KamosoDirModel::KamosoDirModel(QObject* parent)
    : KDirSortFilterProxyModel(parent)
    , m_dirModel(new KDirModel(this))
{
    setSourceModel(m_dirModel);
    sort(KDirModel::ModifiedTime, Qt::DescendingOrder);
    setDynamicSortFilter(true);
}

QHash< int, QByteArray > KamosoDirModel::roleNames() const
{
    QHash<int, QByteArray> roles = KDirSortFilterProxyModel::roleNames();
    roles.insert(Path, "path");
    roles.insert(MimeType, "mime");
    return roles;
}

void KamosoDirModel::setUrl(const QUrl& url)
{
    m_dirModel->dirLister()->openUrl(url, KDirLister::Reload);
}

QUrl KamosoDirModel::url() const
{
    return m_dirModel->dirLister()->url();
}

QString KamosoDirModel::nameFilter() const
{
    return m_dirModel->dirLister()->nameFilter();
}

void KamosoDirModel::setNameFilter(const QString& filter)
{
    m_dirModel->dirLister()->setNameFilter(filter);
}

void KamosoDirModel::setMimeFilter(const QStringList& mimes)
{
    if(mimes==m_dirModel->dirLister()->mimeFilters())
        return;

    m_dirModel->dirLister()->setMimeFilter(mimes);
    if(!m_dirModel->dirLister()->url().isEmpty()) {
        m_dirModel->dirLister()->openUrl(m_dirModel->dirLister()->url(), KDirLister::Reload);
    }
    Q_EMIT filterChanged();
}

QStringList KamosoDirModel::mimeFilter() const
{
    return m_dirModel->dirLister()->mimeFilters();
}

QVariant KamosoDirModel::data(const QModelIndex& index, int role) const
{
    switch(role) {
        case Path:
            return qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole)).url();
        case MimeType:
            return qvariant_cast<KFileItem>(index.data(KDirModel::FileItemRole)).mimetype();
        default:
            return KDirSortFilterProxyModel::data(index, role);
    }
}
