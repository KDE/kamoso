/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <gst/gstdevice.h>

#include <KSharedConfig>

QString structureValue(GstStructure* device, const char* key);
QString objectIdFromProperties(GstStructure* deviceProperties);

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filters READ filters WRITE setFilters NOTIFY filtersChanged)

    public:
        Device(GstDevice* device, QObject* parent);
        ~Device();
        QString description() const { return m_description; }
        QString objectId() const;
        void setFilters(const QString &filters);
        QString filters() const { return m_filters; }

        GstElement* createElement();

        void reset();

    Q_SIGNALS:
        void filtersChanged(const QString &filters);

    private:
        const QString m_description;
        GstDevice *const m_device;
        QString m_objectId;
        QString m_filters;
};

#endif //DEVICE_H
