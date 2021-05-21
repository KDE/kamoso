/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and/or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#ifndef BUFFERFORMAT_H
#define BUFFERFORMAT_H

#include "utils.h"
#include <QSharedData>
#include <gst/video/video.h>

/**
 * This class is a cheap way to represent Caps.
 * Based on QVideoSurfaceFormat.
 */
class BufferFormat
{
public:
    static BufferFormat fromCaps(GstCaps *caps);
    static GstCaps *newCaps(GstVideoFormat format, const QSize & size,
            const Fraction & framerate, const Fraction & pixelAspectRatio);

    inline BufferFormat() : d(new Data) {}

    inline GstVideoInfo videoInfo() const           { return d->videoInfo; }
    inline GstVideoFormat videoFormat() const {  return GST_VIDEO_INFO_FORMAT(&(d->videoInfo)); }
    inline GstVideoColorMatrix colorMatrix() const { return d->videoInfo.colorimetry.matrix; }
    QSize frameSize() const {
        return QSize(GST_VIDEO_INFO_WIDTH(&(d->videoInfo)),
                     GST_VIDEO_INFO_HEIGHT(&(d->videoInfo)));
    }
    Fraction pixelAspectRatio() const {
        return Fraction(GST_VIDEO_INFO_PAR_N(&(d->videoInfo)),
                        GST_VIDEO_INFO_PAR_D(&(d->videoInfo)));
    }

    int bytesPerLine(int component = 0) const;

private:
    friend bool operator==(BufferFormat a, BufferFormat b);
    friend bool operator!=(BufferFormat a, BufferFormat b);

    struct Data : public QSharedData
    {
        Data()
        { gst_video_info_init(&videoInfo); }

        GstVideoInfo videoInfo;
    };
    QSharedDataPointer<Data> d;
};

Q_DECLARE_METATYPE(GstVideoInfo)
Q_DECLARE_METATYPE(GstVideoFormat)
Q_DECLARE_METATYPE(GstVideoColorMatrix)
Q_DECLARE_METATYPE(BufferFormat)

#endif // BUFFERFORMAT_H
