/************************************************************************************
 * Copyright (C) 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>                *
 *                                                                                  *
 * This program is free software; you can redistribute it and/or                    *
 * modify it under the terms of the GNU General Public License                      *
 * as published by the Free Software Foundation; either version 2                   *
 * of the License, or (at your option) any later version.                           *
 *                                                                                  *
 * This program is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 * GNU General Public License for more details.                                     *
 *                                                                                  *
 * You should have received a copy of the GNU General Public License                *
 * along with this program; if not, write to the Free Software                      *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 ************************************************************************************/

#include "sharealternativesmodel.h"
#include <QMimeType>
#include <QMimeDatabase>
#include <QList>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <QIcon>
#include <QDebug>

void ShareAlternativesModel::setMimeTypes(const QStringList& mimes)
{
    if (m_mime == mimes)
        return;

    m_mime = mimes;

    QMimeDatabase db;
    QList<QMimeType> types;
    for(const QString& mime: mimes) {
        types += db.mimeTypeForName(mime);
    }

    beginResetModel();
    m_plugins = KPluginLoader::findPlugins("kamoso/share/", [types](const KPluginMetaData& meta) {
        QRegularExpression rx();
        for(const QMimeType& type: types) {
            QString supported = meta.value("X-Kamoso-ShareMimeType", "*");
            if ((supported.contains('*') && QRegExp(supported, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(type.name())) || type.inherits(supported)) {
                return true;
            }
        }
        return false;
    });
    endResetModel();

    Q_EMIT mimeTypesChanged();
}

QStringList ShareAlternativesModel::mimeTypes() const
{
    return m_mime;
}

ShareJob* ShareAlternativesModel::createJob(int row, const QMimeData& data)
{
    KPluginMetaData pluginData = m_plugins.at(row);
    KPluginLoader loader(pluginData.name(), this);
    Q_ASSERT(loader.errorString().isEmpty());
    return qobject_cast<SharePlugin*>(loader.instance())->share(data);
}

int ShareAlternativesModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_plugins.count();
}

QVariant ShareAlternativesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row()>m_plugins.count())
        return QVariant();

    KPluginMetaData data = m_plugins[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return data.name();
        case Qt::ToolTip:
            return data.description();
        case Qt::DecorationRole:
            return QIcon::fromTheme(data.iconName());
    }
    return QVariant();
}
