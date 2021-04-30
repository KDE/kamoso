/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and /or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#ifndef GENERICSURFACEPAINTER_H
#define GENERICSURFACEPAINTER_H

#include "abstractsurfacepainter.h"
#include <QSet>
#include <QImage>

/**
 * Generic painter that paints using the QPainter API.
 * No colorspace conversion is done and no colors adjustment either.
 */
class GenericSurfacePainter : public AbstractSurfacePainter
{
public:
    GenericSurfacePainter();

    static QSet<GstVideoFormat> supportedPixelFormats();

    virtual bool supportsFormat(GstVideoFormat format) const {
        return supportedPixelFormats().contains(format);
    }

    virtual void init(const BufferFormat &format);
    virtual void cleanup();

    virtual void paint(quint8 *data, const BufferFormat & frameFormat,
                       QPainter *painter, const PaintAreas & areas);

    virtual void updateColors(int brightness, int contrast, int hue, int saturation);

private:
    QImage::Format m_imageFormat;
};

#endif // GENERICSURFACEPAINTER_H
