/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#ifndef DEVICE_H
#define DEVICE_H

#include <QtCore/QObject>

#include <KSharedConfig>

namespace UdevQt {
    class Device;
}
class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filters READ filters WRITE setFilters NOTIFY filtersChanged)

    public:
        Device(const UdevQt::Device &device, QObject* parent = 0);
        ~Device();
        QString description() const;
        QString udi() const;
        QByteArray path() const;
        QString vendor() const;
        void setFilters(const QString &filters);
        QString filters() const;

        void reset();

    Q_SIGNALS:
        void filtersChanged(const QString &filters);

    private:
        const QString m_description;
        const QString m_udi;
        const QByteArray m_path;
        const QString m_vendor;
        QString m_filters;
};

#endif //DEVICE_H
