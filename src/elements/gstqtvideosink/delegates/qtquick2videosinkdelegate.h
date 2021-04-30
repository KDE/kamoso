/*
    SPDX-FileCopyrightText: 2013 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#ifndef QTQUICK2VIDEOSINKDELEGATE_H
#define QTQUICK2VIDEOSINKDELEGATE_H

#include "basedelegate.h"
#include <QtQuick/QSGNode>

class QtQuick2VideoSinkDelegate : public BaseDelegate
{
    Q_OBJECT
public:
    explicit QtQuick2VideoSinkDelegate(GstElement * sink, QObject * parent = 0);

    QSGNode *updateNode(QSGNode *node, const QRectF & targetArea);
};

#endif // QTQUICK2VIDEOSINKDELEGATE_H
