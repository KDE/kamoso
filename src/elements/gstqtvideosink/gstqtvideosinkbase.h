/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and/or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#ifndef GST_QT_VIDEO_SINK_BASE_H
#define GST_QT_VIDEO_SINK_BASE_H

#include "gstqtvideosinkplugin.h"
#include <gst/video/gstvideosink.h>

#define GST_TYPE_QT_VIDEO_SINK_BASE \
  (GstQtVideoSinkBase::get_type())
#define GST_QT_VIDEO_SINK_BASE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_QT_VIDEO_SINK_BASE, GstQtVideoSinkBase))
#define GST_QT_VIDEO_SINK_BASE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_QT_VIDEO_SINK_BASE, GstQtVideoSinkBaseClass))
#define GST_QT_VIDEO_SINK_BASE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_QT_VIDEO_SINK_BASE, GstQtVideoSinkBaseClass))

class QtVideoSinkDelegate;

struct GstQtVideoSinkBase
{
public:
    GstVideoSink parent;

    static GType get_type();

private:
    enum {
        PROP_0,
        PROP_PIXEL_ASPECT_RATIO,
        PROP_FORCE_ASPECT_RATIO,
    };

    static void base_init(gpointer g_class);
    static void class_init(gpointer g_class, gpointer class_data);
    static void init(GTypeInstance *instance, gpointer g_class);
    static void finalize(GObject *object);

    static void set_property(GObject *object, guint prop_id,
                             const GValue *value, GParamSpec *pspec);
    static void get_property(GObject *object, guint prop_id,
                             GValue *value, GParamSpec *pspec);

    static GstStateChangeReturn change_state(GstElement *element, GstStateChange transition);

    static gboolean set_caps(GstBaseSink *sink, GstCaps *caps);

    static GstFlowReturn show_frame(GstVideoSink *sink, GstBuffer *buffer);

public:
    QtVideoSinkDelegate *delegate;

private:
    static GstVideoSinkClass *s_parent_class;
};


struct GstQtVideoSinkBaseClass
{
    GstVideoSinkClass parent_class;
};

#endif
