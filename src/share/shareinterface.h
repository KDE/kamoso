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

#ifndef SHAREINTERFACE_H
#define SHAREINTERFACE_H

#include <KJob>
#include <QVariantMap>
#include <QMimeData>

class Q_DECL_EXPORT ShareJob : public KJob
{
Q_OBJECT
public:
    void setData(const QVariantMap& data);
    QVariantMap data() const;

Q_SIGNALS:
    void needInteraction(const QUrl& path);
    void output(const QVariant& output);
};

class Q_DECL_EXPORT SharePlugin : public QObject
{
    Q_OBJECT
    public:
        virtual ~SharePlugin();

        /** @returns the job that will perform the share of the specified @p data.*/
        virtual ShareJob* share(const QMimeData& data) const = 0;
};

#endif
