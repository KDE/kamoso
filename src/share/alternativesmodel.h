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

#ifndef SHAREALTERNATIVESMODEL_H
#define SHAREALTERNATIVESMODEL_H

#include <QAbstractListModel>
#include <KPluginMetaData>

namespace Purpose
{
class Job;

/**
 * @short Interface for client applications to share data
 *
 * Lists all the alternatives to share a specified type of data then provides
 * a method to trigger a job.
 */
class Q_DECL_EXPORT AlternativesModel : public QAbstractListModel
{
Q_OBJECT
Q_PROPERTY(QString pluginType READ pluginType WRITE setPluginType NOTIFY pluginTypeChanged);
Q_PROPERTY(QJsonObject inputData READ inputData WRITE setInputData NOTIFY inputDataChanged);
public:
    QJsonObject inputData() const;
    void setInputData(const QJsonObject& input);

    QString pluginType() const;
    void setPluginType(const QString& pluginType);

    /**
     * This shouldn't require to have the job actually running on the same process as the app.
     *
     * @param row specifies the alternative to be used
     * @param data specifies the data to have shared
     *
     * @returns the share job.
     */
    Q_SCRIPTABLE Purpose::Job* createJob(int row);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

Q_SIGNALS:
    void inputDataChanged();
    void pluginTypeChanged();

private:
    void initializeModel();

#warning move into a d-pointer
    QVector<KPluginMetaData> m_plugins;
    QJsonObject m_inputData;
    QString m_pluginType;
    QJsonObject m_pluginTypeData;
};

}

#endif
