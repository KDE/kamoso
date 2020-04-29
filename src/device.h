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

#include <QObject>
#include <gst/gstdevice.h>

#include <KSharedConfig>

QString structureValue(GstStructure* device, const char* key);

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filters READ filters WRITE setFilters NOTIFY filtersChanged)

    public:
        Device(GstDevice* device, QObject* parent);
        ~Device();
        QString description() const { return m_description; }
        QString udi() const { return m_udi; }
        QString path() const { return m_path; }
        void setFilters(const QString &filters);
        QString filters() const { return m_filters; }

        GstElement* createElement();

        bool isValid() const;
        void reset();

    Q_SIGNALS:
        void filtersChanged(const QString &filters);

    private:
        const QString m_description;
        GstDevice *const m_device;
        QString m_udi;
        QString m_path;
        QString m_filters;
};

#endif //DEVICE_H
