/*
    SPDX-FileCopyrightText: 2012-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef QGST_QUICK_VIDEOSURFACE_H
#define QGST_QUICK_VIDEOSURFACE_H

#include <gst/gstelement.h>
#include <QObject>

namespace QGst {
namespace Quick {

class VideoSurfacePrivate;

/*!
 * NOTE: File copied over from QtGStreamer when I was told it's deprecated
 *
 * \brief Helper class for painting video on a QtQuick2 VideoItem
 *
 * This is a helper class that represents a video surface on a QQuickView.
 * To use it, create a VideoItem from QML and connect it with this surface.
 *
 * Example:
 * \code
 * // in your C++ code
 * QQuickView *view = new QQuickView;
 * ...
 * QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface;
 * view->rootContext()->setContextProperty(QLatin1String("videoSurface"), surface);
 * ...
 * // and in your qml file:
 * import QtGStreamer 1.0
 * ...
 * VideoItem {
 *      id: video
 *      width: 320
 *      height: 240
 *      surface: videoSurface
 * }
 * \endcode
 *
 * \sa VideoItem
 */
class VideoSurface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(VideoSurface)
public:
    explicit VideoSurface(QObject *parent = 0);
    virtual ~VideoSurface();

    /*! Returns the video sink element that provides this surface's image.
     * The element will be constructed the first time that this function
     * is called. The surface will always keep a reference to this element.
     */
    GstElement* videoSink() const;

    void onUpdate();

private:
    friend class VideoItem;
    VideoSurfacePrivate * const d;
};

} // namespace Quick
} // namespace QGst

Q_DECLARE_METATYPE(QGst::Quick::VideoSurface*)

#endif // QGST_QUICK_VIDEOSURFACE_H
