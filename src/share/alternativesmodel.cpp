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

#include "alternativesmodel.h"
#include <QMimeType>
#include <QMimeDatabase>
#include <QList>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <KPluginFactory>
#include <QIcon>
#include <QDebug>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QFile>
#include <QJsonArray>
#include <QRegularExpression>

#include "pluginbase.h"
#include "job.h"

using namespace Purpose;

void AlternativesModel::setInputData(const QJsonObject &input)
{
    if (input == m_inputData)
        return;

    m_inputData = input;
    initializeModel();

    Q_EMIT inputDataChanged();
}

void AlternativesModel::setPluginType(const QString& pluginType)
{
    if (pluginType == m_pluginType)
        return;

    const QString lookup = QStringLiteral("purpose/types/") + pluginType + QStringLiteral("PluginType.json");
    const QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, lookup);
    if (path.isEmpty()) {
        qWarning() << "Couldn't find" << lookup;
        return;
    }
    QFile typeFile(path);
    if (!typeFile.open(QFile::ReadOnly)) {
        qWarning() << "Couldn't open" << lookup;
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(typeFile.readAll(), &error);
    if (error.error) {
        qWarning() << "JSON error in " << path << error.offset << ":" << error.errorString();
        return;
    }

    Q_ASSERT(doc.isObject());
    QJsonObject typeData = doc.object();
    m_pluginTypeData = typeData;
    m_pluginType = pluginType;
    Q_ASSERT(m_pluginTypeData.isEmpty() == m_pluginType.isEmpty());

    initializeModel();

    Q_EMIT pluginTypeChanged();
}

QString AlternativesModel::pluginType() const
{
    return m_pluginType;
}

QJsonObject AlternativesModel::inputData() const
{
    return m_inputData;
}

Purpose::Job* AlternativesModel::createJob(int row)
{
    KPluginMetaData pluginData = m_plugins.at(row);
    KPluginLoader loader(pluginData.fileName(), this);
    Purpose::PluginBase* plugin = dynamic_cast<Purpose::PluginBase*>(loader.factory()->create<QObject>(this, QVariantList()));

    if (!plugin) {
        qWarning() << "Couldn't load plugin:" << pluginData.fileName() << loader.errorString();
    }

    Purpose::Job* job = plugin->share();
    job->setParent(this);
    job->setData(m_inputData);
    job->setConfigurationArguments(m_inputData.value("X-Purpose-InboundArguments").toString().split(',', QString::SkipEmptyParts));
    job->setInboundArguments(pluginData.value("X-Purpose-Configuration").split(',', QString::SkipEmptyParts));
    connect(job, &Purpose::Job::finished, plugin, &QObject::deleteLater); //TODO only instantiate 1 plugin factory per type
    return job;
}

int AlternativesModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_plugins.count();
}

QVariant AlternativesModel::data(const QModelIndex& index, int role) const
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

typedef bool (*matchFunction)(const QString& constraint, const QJsonValue& value);

static bool defaultMatch(const QString& constraint, const QJsonValue& value)
{
    return value == QJsonValue(constraint);
}

static bool mimeTypeMatch(const QString& constraint, const QJsonValue& value)
{
    if(value.isArray()) {
        foreach(const QJsonValue& val, value.toArray()) {
            if (mimeTypeMatch(constraint, val))
                return true;
        }
        return false;
    } else if(value.isObject()) {
        foreach(const QJsonValue& val, value.toObject()) {
            if (mimeTypeMatch(constraint, val))
                return true;
        }
        return false;
    } else if(constraint.contains('*')) {
        return QRegExp(constraint, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(value.toString());
    } else {
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForName(value.toString());
        return mime.inherits(constraint);
    }
}

static QMap<QString, matchFunction> s_matchFunctions = {
    { QStringLiteral("mimeType"), mimeTypeMatch }
};

void AlternativesModel::initializeModel()
{
    if (m_pluginType.isEmpty()) {
        return;
    }
    QStringList inbound = m_pluginTypeData.value("X-Purpose-InboundArguments").toString().split(',', QString::SkipEmptyParts);
    foreach(const QString& arg, inbound) {
        if(!m_inputData.contains(arg)) {
            qWarning() << "Cannot initialize model with data" << m_inputData << ". missing:" << arg;
            return;
        }
    }

//     TODO: allow proper list stuff instead of splitting
    beginResetModel();
    m_plugins = KPluginLoader::findPlugins("purpose", [this](const KPluginMetaData& meta) {
        if(!meta.value("X-Purpose-PluginTypes").split(',').contains(m_pluginType)) {
//             qDebug() << "discarding" << meta.name() << meta.value("X-Purpose-PluginTypes");
            return false;
        }

        QStringList constraints = meta.value("X-Purpose-Constraints").split(',', QString::SkipEmptyParts);
        for(const QString& constraint: constraints) {
            static const QRegularExpression constraintRx("(\\w+):(.*)");
            Q_ASSERT(constraintRx.isValid());
            QRegularExpressionMatch match = constraintRx.match(constraint);
            if (!match.isValid() || !match.hasMatch()) {
                qWarning() << "wrong constraint" << constraint;
                continue;
            }
            QString propertyName = match.captured(1);
            QString constrainedValue = match.captured(2);
            bool acceptable = s_matchFunctions.value(propertyName, defaultMatch)(constrainedValue, m_inputData[propertyName]);
            if (!acceptable) {
//                 qDebug() << "not accepted" << meta.name() << propertyName << constrainedValue << constraint;
                return false;
            }
        }
        return true;
    });
    endResetModel();

}
