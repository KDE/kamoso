/*
    SPDX-FileCopyrightText: 2010 Nokia Corporation and/or its subsidiary(-ies). <qt-info@nokia.com>
    SPDX-FileCopyrightText: 2011-2013 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#include "qtquick2videosinkdelegate.h"
#include "../painters/videonode.h"

QtQuick2VideoSinkDelegate::QtQuick2VideoSinkDelegate(GstElement *sink, QObject *parent)
    : BaseDelegate(sink, parent)
{
}

QSGNode* QtQuick2VideoSinkDelegate::updateNode(QSGNode *node, const QRectF & targetArea)
{
    GST_TRACE_OBJECT(m_sink, "updateNode called");
    bool sgnodeFormatChanged = false;

    VideoNode *vnode = dynamic_cast<VideoNode*>(node);
    if (!vnode) {
        GST_INFO_OBJECT(m_sink, "creating new VideoNode");
        vnode = new VideoNode;
        m_formatDirty = true;
    }

    if (!m_buffer) {
        if (vnode->materialType() != VideoNode::MaterialTypeSolidBlack) {
            vnode->setMaterialTypeSolidBlack();
            sgnodeFormatChanged = true;
        }
        if (sgnodeFormatChanged || targetArea != m_areas.targetArea || !vnode->geometry()) {
            m_areas.targetArea = targetArea;
            vnode->updateGeometry(m_areas);
        }
    } else {
        //change format before geometry, so that we change QSGGeometry as well
        if (m_formatDirty) {
            vnode->changeFormat(m_bufferFormat);
            sgnodeFormatChanged = true;
        }

        //recalculate the video area if needed
        QReadLocker forceAspectRatioLocker(&m_forceAspectRatioLock);
        if (sgnodeFormatChanged || targetArea != m_areas.targetArea || m_forceAspectRatioDirty) {
            m_forceAspectRatioDirty = false;

            QReadLocker pixelAspectRatioLocker(&m_pixelAspectRatioLock);
            Qt::AspectRatioMode aspectRatioMode = m_forceAspectRatio ?
                    Qt::KeepAspectRatio : Qt::IgnoreAspectRatio;
            m_areas.calculate(targetArea, m_bufferFormat.frameSize(),
                    m_bufferFormat.pixelAspectRatio(), m_pixelAspectRatio,
                    aspectRatioMode);
            pixelAspectRatioLocker.unlock();

            GST_LOG_OBJECT(m_sink,
                "Recalculated paint areas: "
                "Frame size: " QSIZE_FORMAT ", "
                "target area: " QRECTF_FORMAT ", "
                "video area: " QRECTF_FORMAT ", "
                "black1: " QRECTF_FORMAT ", "
                "black2: " QRECTF_FORMAT,
                QSIZE_FORMAT_ARGS(m_bufferFormat.frameSize()),
                QRECTF_FORMAT_ARGS(m_areas.targetArea),
                QRECTF_FORMAT_ARGS(m_areas.videoArea),
                QRECTF_FORMAT_ARGS(m_areas.blackArea1),
                QRECTF_FORMAT_ARGS(m_areas.blackArea2)
            );

            vnode->updateGeometry(m_areas);
        }
        forceAspectRatioLocker.unlock();

        if (m_formatDirty) {
            m_formatDirty = false;

            //make sure to update the colors after changing material
            m_colorsDirty = true;
        }

        QReadLocker colorsLocker(&m_colorsLock);
        if (m_colorsDirty) {
            vnode->updateColors(m_brightness, m_contrast, m_hue, m_saturation);
            m_colorsDirty = false;
        }
        colorsLocker.unlock();

        vnode->setCurrentFrame(m_buffer);
    }

    return vnode;
}
