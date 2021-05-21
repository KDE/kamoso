/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and/or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2012 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#ifndef GST_QT_GL_VIDEO_SINK_BASE_H
#define GST_QT_GL_VIDEO_SINK_BASE_H

#include "gstqtvideosinkbase.h"
#include <gst/video/colorbalance.h>

#define GST_TYPE_QT_GL_VIDEO_SINK_BASE \
  (GstQtGLVideoSinkBase::get_type())
#define GST_QT_GL_VIDEO_SINK_BASE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_QT_GL_VIDEO_SINK_BASE, GstQtGLVideoSinkBase))

struct GstQtGLVideoSinkBase
{
public:
    GstQtVideoSinkBase parent;

    static GType get_type();

private:
    enum {
        PROP_0,
        PROP_CONTRAST,
        PROP_BRIGHTNESS,
        PROP_HUE,
        PROP_SATURATION
    };

    //index for s_colorbalance_labels
    enum {
        LABEL_CONTRAST = 0,
        LABEL_BRIGHTNESS,
        LABEL_HUE,
        LABEL_SATURATION,
        LABEL_LAST
    };

    static void init_interfaces(GType type);

    static void base_init(gpointer g_class);
    static void class_init(gpointer g_class, gpointer class_data);

    static void init(GTypeInstance *instance, gpointer g_class);
    static void finalize(GObject *object);

    static void colorbalance_init(GstColorBalanceInterface *balance_interface, gpointer data);
    static const GList *colorbalance_list_channels(GstColorBalance *balance);
    static void colorbalance_set_value(GstColorBalance *balance,
                                       GstColorBalanceChannel *channel,
                                       gint value);
    static gint colorbalance_get_value(GstColorBalance *balance,
                                       GstColorBalanceChannel *channel);
    static GstColorBalanceType colorbalance_get_balance_type(GstColorBalance *balance);

    static void set_property(GObject *object, guint prop_id,
                             const GValue *value, GParamSpec *pspec);
    static void get_property(GObject *object, guint prop_id,
                             GValue *value, GParamSpec *pspec);

    static gboolean start(GstBaseSink *sink);
    static gboolean set_caps(GstBaseSink *sink, GstCaps *caps);


    GList *m_channels_list;
    static const char * const s_colorbalance_labels[];
    static GstQtVideoSinkBaseClass *s_parent_class;
};


struct GstQtGLVideoSinkBaseClass
{
    GstQtVideoSinkBaseClass parent_class;
};

#endif // GST_QT_GL_VIDEO_SINK_BASE_H
