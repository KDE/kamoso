/*
    SPDX-FileCopyrightText: 2012-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "videoitem.h"
#include "videosurface_p.h"
#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

namespace QGst {
namespace Quick {

struct VideoItem::Private
{
    QPointer<VideoSurface> surface;
    bool surfaceDirty;
    QRectF targetArea;
};

VideoItem::VideoItem(QQuickItem *parent)
    : QQuickItem(parent), d(new Private)
{
    d->surfaceDirty = true;
    setFlag(QQuickItem::ItemHasContents, true);
}

VideoItem::~VideoItem()
{
    setSurface(0);
    delete d;
}

VideoSurface *VideoItem::surface() const
{
    return d->surface.data();
}

void VideoItem::setSurface(VideoSurface *surface)
{
    if (d->surface) {
        d->surface.data()->d->items.remove(this);
    }

    d->surface = surface;
    d->surfaceDirty = true;

    if (d->surface) {
        d->surface.data()->d->items.insert(this);
    }
}

#define G_TYPE_QREAL        ((sizeof(qreal) == sizeof(double)) ? G_TYPE_DOUBLE : G_TYPE_FLOAT)

#if defined(QT_COORD_TYPE)
#define g_value_set_qreal   g_value_set_##QT_COORD_TYPE
#else
#define g_value_set_qreal g_value_set_double
#endif

QSGNode* VideoItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)

    QRectF r = boundingRect();
    QSGNode *newNode = nullptr;


    if (d->surfaceDirty) {
        delete oldNode;
        oldNode = 0;
        d->surfaceDirty = false;
    }

    if (!d->surface || !d->surface.data()->d->videoSink) {
        if (!oldNode) {
            QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
            material->setColor(Qt::black);

            QSGGeometryNode *node = new QSGGeometryNode;
            node->setMaterial(material);
            node->setFlag(QSGNode::OwnsMaterial);
            node->setFlag(QSGNode::OwnsGeometry);

            newNode = node;
            d->targetArea = QRectF(); //force geometry to be set
        } else {
            newNode = oldNode;
        }

        if (r != d->targetArea) {
            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
            geometry->vertexDataAsPoint2D()[0].set(r.x(), r.y());
            geometry->vertexDataAsPoint2D()[1].set(r.x(), r.height());
            geometry->vertexDataAsPoint2D()[2].set(r.width(), r.y());
            geometry->vertexDataAsPoint2D()[3].set(r.width(), r.height());

            QSGGeometryNode *node = static_cast<QSGGeometryNode*>(newNode);
            node->setGeometry(geometry);

            d->targetArea = r;
        }
    } else {
        const int count = 6; //this + oldNode + rect
        GValue *values = new GValue[count + 1];
        memset(values, 0, sizeof(GValue) * (count + 1));

        g_value_init(&values[0], G_TYPE_POINTER);
        g_value_set_pointer(&values[0], d->surface.data()->d->videoSink);

        g_value_init(&values[1], G_TYPE_POINTER);
        g_value_set_pointer(&values[1], oldNode);

        int i=2;
        for (auto v : {r.x(), r.y(), r.width(), r.height()}) {
            g_value_init(&values[i], G_TYPE_QREAL);
            g_value_set_qreal(&values[i], v);
            i++;
        }

        GValue returnValue = {0, {{0}, {0}}};
        g_value_init(&returnValue, G_TYPE_POINTER);

        //emit the signal
        auto itype = G_TYPE_FROM_INSTANCE(d->surface.data()->d->videoSink);
        g_signal_emitv(values, g_signal_lookup("update-node", itype), 0, &returnValue);

        //cleanup
        for(int i=0; i<count + 1; i++) {
            g_value_unset(&values[i]);
        }
        delete[] values;
        newNode = (QSGNode*) g_value_get_pointer(&returnValue);
    }

    return newNode;
}

} // namespace Quick
} // namespace QGst
