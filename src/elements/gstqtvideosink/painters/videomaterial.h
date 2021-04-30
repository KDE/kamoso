/*
    SPDX-FileCopyrightText: 2013 Digia Plc and /or its subsidiary(-ies).
    SPDX-FileCopyrightText: 2013 basysKom GmbH <info@basyskom.com>
    SPDX-FileCopyrightText: 2013 Collabora Ltd. <info@collabora.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/

#ifndef VIDEOMATERIAL_H
#define VIDEOMATERIAL_H

#include "../utils/bufferformat.h"
#include <QSize>
#include <QMutex>
#include <QMatrix4x4>

#include <QSGMaterial>
#include <QOpenGLFunctions>

class VideoMaterialShader;

class VideoMaterial : public QSGMaterial, public QOpenGLFunctions
{
public:
    static VideoMaterial *create(const BufferFormat & format);

    virtual ~VideoMaterial();

    virtual int compare(const QSGMaterial *other) const;

    void setCurrentFrame(GstBuffer *buffer);
    void updateColors(int brightness, int contrast, int hue, int saturation);

    void bind();

protected:
    VideoMaterial();
    void initRgbTextureInfo(GLenum internalFormat, GLuint format,
                            GLenum type, const QSize &size);
    void initYuv420PTextureInfo(bool uvSwapped, const QSize &size);
    void init(GstVideoColorMatrix colorMatrixType);

private:
    void bindTexture(int i, const quint8 *data);


    GstBuffer *m_frame;
    QMutex m_frameMutex;

    static const int Num_Texture_IDs = 3;
    int m_textureCount;
    GLuint m_textureIds[Num_Texture_IDs];
    int m_textureWidths[Num_Texture_IDs];
    int m_textureHeights[Num_Texture_IDs];
    int m_textureOffsets[Num_Texture_IDs];
    QSize m_textureSize;

    GLenum m_textureFormat;
    GLuint m_textureInternalFormat;
    GLenum m_textureType;

    QMatrix4x4 m_colorMatrix;
    GstVideoColorMatrix m_colorMatrixType;

    friend class VideoMaterialShader;
};

#endif // VIDEOMATERIAL_H
