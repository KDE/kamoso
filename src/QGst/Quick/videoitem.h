/*
    SPDX-FileCopyrightText: 2012-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef QGST_QUICK_VIDEOITEM_H
#define QGST_QUICK_VIDEOITEM_H

#include "videosurface.h"
#include <QtQuick/QQuickItem>
#include <QPointer>

namespace QGst {
namespace Quick {

/*! \headerfile videoitem.h <QGst/Quick/VideoItem>
 * \brief A QQuickItem for displaying video
 *
 * This is a QQuickItem subclass that can display video. To use it,
 * you have to create a VideoSurface and connect it with this
 * item using the setSurface() method or the surface property.
 * See the VideoSurface documentation for details and examples.
 *
 * \sa VideoSurface
 */
class VideoItem : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(VideoItem)
    Q_PROPERTY(QGst::Quick::VideoSurface* surface READ surface WRITE setSurface)

public:
    explicit VideoItem(QQuickItem *parent = 0);
    virtual ~VideoItem();

    VideoSurface *surface() const;
    void setSurface(VideoSurface *surface);

protected:
    virtual QSGNode* updatePaintNode(QSGNode *oldNode,
                                     UpdatePaintNodeData *updatePaintNodeData) override;

private:
    struct Private;
    Private *const d;
};

} // namespace Quick
} // namespace QGst

#endif // QGST_QUICK_VIDEOITEM_H
