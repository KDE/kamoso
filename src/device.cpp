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

QString withoutHexPrefix(QString value)
{
    if (value.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive)) {
        value.remove(0, 2);
    }
    return value;
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
    , m_description(QString::fromUtf8(gst_device_get_display_name(device)))
    , m_device(device)
{
    auto st = gst_device_get_properties(device);
    m_objectId = objectIdFromProperties(st);
    const auto vendorId = withoutHexPrefix(structureValue(st, "device.vendor.id"));
    const auto productId = withoutHexPrefix(structureValue(st, "device.product.id"));
    // This USB2 UVC camera's high-resolution raw modes are too slow and can reset it.
    m_requiresSafeRawMode = vendorId.compare(QStringLiteral("04f2"), Qt::CaseInsensitive) == 0
        && productId.compare(QStringLiteral("b65e"), Qt::CaseInsensitive) == 0;
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
