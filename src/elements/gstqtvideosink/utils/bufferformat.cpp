/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and /or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#include "bufferformat.h"
#include <QByteArray>

BufferFormat BufferFormat::fromCaps(GstCaps *caps)
{
    BufferFormat result;
    if (caps && gst_video_info_from_caps(&(result.d->videoInfo), caps)) {
        return result;
    } else {
        return BufferFormat();
    }
}

GstCaps* BufferFormat::newCaps(GstVideoFormat format, const QSize & size,
                               const Fraction & framerate, const Fraction & pixelAspectRatio)
{
    GstVideoInfo videoInfo;
    gst_video_info_init(&videoInfo);
    gst_video_info_set_format(&videoInfo, format, size.width(), size.height());

    videoInfo.fps_n = framerate.numerator;
    videoInfo.fps_d = framerate.denominator;

    videoInfo.par_n = pixelAspectRatio.numerator;
    videoInfo.par_d = pixelAspectRatio.denominator;

    return gst_video_info_to_caps(&videoInfo);
}

int BufferFormat::bytesPerLine(int component) const
{
    return GST_VIDEO_INFO_PLANE_STRIDE(&(d->videoInfo), component);
}

bool operator==(BufferFormat a, BufferFormat b)
{
    return a.d == b.d;
}

bool operator!=(BufferFormat a, BufferFormat b)
{
    return a.d != b.d;
}
