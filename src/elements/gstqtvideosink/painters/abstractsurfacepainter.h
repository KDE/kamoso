/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and /or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#ifndef ABSTRACTSURFACEPAINTER_H
#define ABSTRACTSURFACEPAINTER_H

#include "../utils/bufferformat.h"
#include <QRectF>

class QPainter;

/** Common interface for all the painters */
class AbstractSurfacePainter
{
public:
    virtual ~AbstractSurfacePainter() {}

    virtual bool supportsFormat(GstVideoFormat format) const = 0;

    virtual void init(const BufferFormat & format) = 0;
    virtual void cleanup() = 0;

    virtual void paint(quint8 *data, const BufferFormat & frameFormat,
                       QPainter *painter, const PaintAreas & areas) = 0;

    virtual void updateColors(int brightness, int contrast, int hue, int saturation) = 0;
};

#endif
