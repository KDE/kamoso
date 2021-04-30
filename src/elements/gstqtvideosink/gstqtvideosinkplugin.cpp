/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and /or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#include "gstqtvideosinkplugin.h"
#include "gstqtquick2videosink.h"

GST_DEBUG_CATEGORY(gst_qt_video_sink_debug);

#pragma GCC visibility push(default)

/* entry point to initialize the plug-in */
static gboolean plugin_init(GstPlugin *plugin)
{
    GST_DEBUG_CATEGORY_INIT(gst_qt_video_sink_debug,
                            G_STRINGIFY(QTVIDEOSINK_NAME), 0,
                            "Debug category for GstQtVideoSink");

    if (!gst_element_register(plugin, "qtquick2videosink",
                GST_RANK_NONE, GST_TYPE_QT_QUICK2_VIDEO_SINK)) {
        GST_ERROR("Failed to register qtquick2videosink");
        return FALSE;
    }

    return TRUE;
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    QTVIDEOSINK_NAME,
    "A video sink that can draw on any Qt surface",
    plugin_init,
    PACKAGE_VERSION,
    "LGPL",
    PACKAGE_NAME,
    PACKAGE_ORIGIN
)
#pragma GCC visibility pop
