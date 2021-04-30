/*
    SPDX-FileCopyrightText: 2012-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include "videosurface_p.h"

#include <QDebug>
#include <QQuickItem>

namespace QGst {
namespace Quick {

static void updateCallback(void*, VideoSurface* vs) { vs->onUpdate(); }

VideoSurface::VideoSurface(QObject *parent)
    : QObject(parent), d(new VideoSurfacePrivate)
{
}

VideoSurface::~VideoSurface()
{
    if (d->updateHandler)
        g_signal_handler_disconnect(d->videoSink, d->updateHandler);

    if (d->videoSink) {
        gst_element_set_state(d->videoSink, GST_STATE_NULL);
        gst_object_unref(d->videoSink);
    }

    delete d;
}

GstElement* VideoSurface::videoSink() const
{
    if (!d->videoSink) {
        d->videoSink = gst_element_factory_make("qtquick2videosink", "qtquick2videosink");

        if (d->videoSink) {
            gst_object_ref_sink(d->videoSink);
        } else {
            qCritical("Failed to create qtquick2videosink. Make sure it is installed correctly");
            return nullptr;
        }

        Q_ASSERT(GST_IS_ELEMENT(d->videoSink));
        Q_ASSERT(G_IS_OBJECT(d->videoSink));

        d->updateHandler = g_signal_connect(d->videoSink, "update", G_CALLBACK(updateCallback), (gpointer) this);
    }

    return d->videoSink;
}

void VideoSurface::onUpdate()
{
    Q_FOREACH(QQuickItem *item, d->items) {
        item->update();
    }
}

} // namespace Quick
} // namespace QGst
