/*
    SPDX-FileCopyrightText: 2013 Digia Plc and /or its subsidiary(-ies).
    SPDX-FileCopyrightText: 2013 basysKom GmbH <info@basyskom.com>
    SPDX-FileCopyrightText: 2013 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#ifndef VIDEONODE_H
#define VIDEONODE_H

#include "../utils/bufferformat.h"

#include <QtQuick/QSGGeometryNode>

class VideoNode : public QSGGeometryNode
{
public:
    VideoNode();

    enum MaterialType {
        MaterialTypeVideo,
        MaterialTypeSolidBlack
    };

    MaterialType materialType() const { return m_materialType; }

    void changeFormat(const BufferFormat &format);
    void setMaterialTypeSolidBlack();

    void setCurrentFrame(GstBuffer *buffer);
    void updateColors(int brightness, int contrast, int hue, int saturation);

    void updateGeometry(const PaintAreas & areas);

private:
    MaterialType m_materialType;
    bool m_validGeometry;
};

#endif // VIDEONODE_H
