/*
    Copyright (C) 2012-2013 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "videosurface_p.h"

#include <QDebug>
#include <QQuickItem>

namespace QGst {
namespace Quick {

VideoSurface::VideoSurface(QObject *parent)
    : QObject(parent), d(new VideoSurfacePrivate)
{
}

VideoSurface::~VideoSurface()
{
    if (d->videoSink) {
        gst_element_set_state(d->videoSink, GST_STATE_NULL);
        gst_object_unref(d->videoSink);
    }

    delete d;
}

static void updateCallback(void*, VideoSurface* vs)
{
    vs->onUpdate();
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

        g_signal_connect(d->videoSink, "update", G_CALLBACK(updateCallback), (gpointer) this);
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
