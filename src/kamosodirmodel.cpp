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
