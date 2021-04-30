/*
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#ifndef UTILS_H
#define UTILS_H

#include <QRectF>
#include <QSize>
#include <QMetaType>

// utilities for GST_DEBUG
#define QSIZE_FORMAT "(%d x %d)"
#define QSIZE_FORMAT_ARGS(size) \
    size.width(), size.height()
#define QRECTF_FORMAT "(x: %f, y: %f, w: %f, h: %f)"
#define QRECTF_FORMAT_ARGS(rect) \
    (float) rect.x(), (float) rect.y(), (float) rect.width(), (float) rect.height()

struct Fraction
{
    inline Fraction() {}
    inline Fraction(int numerator, int denominator)
        : numerator(numerator), denominator(denominator) {}

    inline bool operator==(const Fraction & other) const
    { return numerator == other.numerator && denominator == other.denominator; }
    inline bool operator!=(const Fraction & other) const
    { return !operator==(other); }

    inline qreal ratio() const
    { return (qreal) numerator / (qreal) denominator; }
    inline qreal invRatio() const
    { return (qreal) denominator / (qreal) numerator; }

    int numerator;
    int denominator;
};

struct PaintAreas
{
    void calculate(const QRectF & targetArea,
                   const QSize & videoSize,
                   const Fraction & pixelAspectRatio,
                   const Fraction & displayAspectRatio,
                   Qt::AspectRatioMode aspectRatioMode);

    // the area that we paint on
    QRectF targetArea;
    // the area where the video should be painted on
    // (subrect of or equal to targetArea)
    QRectF videoArea;

    // the part of the video rectangle that we are going to blit on the videoArea
    // in the normalized (0,1] range (texture coordinates)
    QRectF sourceRect;

    // these are small subrects of targetArea that are not
    // covered by videoArea to keep the video's aspect ratio
    QRectF blackArea1;
    QRectF blackArea2;
};

Q_DECLARE_METATYPE(Fraction)
Q_DECLARE_METATYPE(PaintAreas)

#endif
