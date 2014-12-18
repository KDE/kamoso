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

#include "shareinterface.h"
#include <QDebug>

struct ShareJobPrivate
{
    QJsonObject m_data;
    QStringList requiredArguments;
    QStringList optionalArguments;
};

ShareJob::ShareJob(QObject* parent)
    : KJob(parent)
    , d_ptr(new ShareJobPrivate)
{
}

ShareJob::~ShareJob()
{
    delete d_ptr;
}

SharePlugin::~SharePlugin()
{
}

QJsonObject ShareJob::data() const
{
    Q_D(const ShareJob);
    return d->m_data;
}

void ShareJob::setData(const QJsonObject& data)
{
    Q_D(ShareJob);

    qDebug() << "datachanged" << data;
    if (d->m_data != data) {
        d->m_data = data;
        emit dataChanged();
    }
}

SharePlugin::SharePlugin(QObject* parent)
    : QObject(parent)
{
}

bool ShareJob::isReady() const
{
    Q_D(const ShareJob);
    for(const QString& arg: d->requiredArguments + d->optionalArguments) {
        if(!d->m_data.contains(arg))
            return false;
    }
    return true;
}

void ShareJob::setAdditionalArguments(const QStringList& args)
{
    Q_D(ShareJob);
    d->optionalArguments = args;
}

void ShareJob::setMandatoryArguments(const QStringList& args)
{
    Q_D(ShareJob);
    d->requiredArguments = args;
}

QStringList ShareJob::acceptedArguments() const
{
    Q_D(const ShareJob);
    return d->requiredArguments + d->optionalArguments;
}
