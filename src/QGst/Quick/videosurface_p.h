/*
    SPDX-FileCopyrightText: 2012-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef QGST_QUICK_VIDEOSURFACE_P_H
#define QGST_QUICK_VIDEOSURFACE_P_H

#include "videosurface.h"
#include "videoitem.h"

namespace QGst {
namespace Quick {

class VideoSurfacePrivate
{
public:
    QSet<VideoItem*> items;
    GstElement* videoSink = nullptr;
    int updateHandler = 0;
};

} // namespace Quick
} // namespace QGst

#endif // QGST_QUICK_VIDEOSURFACE_P_H
