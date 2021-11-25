/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "device.h"
#include <KConfigGroup>
#include <QDebug>

QString structureValue(GstStructure* device, const char* key)
{
    auto x = gst_structure_get_value(device, key);
    if (!x)
        return {};
    return QString::fromUtf8(g_value_get_string(x));
}

QString objectIdFromProperties(GstStructure* st)
{
    // The value returned here is later used to identify
    // the camera device we are dealing with.
    QString objectId = structureValue(st, "object.id");
    if (!objectId.isEmpty()) {
        return objectId;
    } else {
        // fallback value in-case the above returns empty string.
        return structureValue(st, "device.path");
    }
}

//     for reference, the properties can be listed with:
//     gst-device-monitor-1.0 Video/Source
Device::Device(GstDevice *device, QObject* parent)
    : QObject(parent)
    , m_description(gst_device_get_display_name(device))
    , m_device(device)
{
    auto st = gst_device_get_properties(device);
    m_objectId = objectIdFromProperties(st);
    gst_structure_free(st);
    setObjectName(m_objectId);
}

Device::~Device()
{}

void Device::reset()
{
    m_filters.clear();

    Q_EMIT filtersChanged(m_filters);
}

QString Device::objectId() const
{
    return m_objectId;
}

void Device::setFilters(const QString &newFilters)
{
    if (newFilters == m_filters) {
        return;
    }

    m_filters = newFilters;
    Q_EMIT filtersChanged(newFilters);
}

GstElement* Device::createElement()
{
    return gst_device_create_element(m_device, m_description.toUtf8().constData());
}
