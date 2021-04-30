/*
    SPDX-FileCopyrightText: 2011-2013 Collabora Ltd. <info@collabora.com>
    SPDX-FileCopyrightText: 2013 Digia Plc and /or its subsidiary(-ies).
    SPDX-FileCopyrightText: 2013 basysKom GmbH <info@basyskom.com>

    SPDX-License-Identifier: LGPL-2.1-only
*/
#include "utils.h"

void PaintAreas::calculate(const QRectF & targetArea,
        const QSize & videoSize,
        const Fraction & pixelAspectRatio,
        const Fraction & displayAspectRatio,
        Qt::AspectRatioMode aspectRatioMode)
{
    this->targetArea = targetArea;

    switch (aspectRatioMode) {
    case Qt::IgnoreAspectRatio:
        videoArea = targetArea;
        sourceRect = QRectF(0, 0, 1, 1);
        blackArea1 = blackArea2 = QRectF();
        break;
    default:
      {
        qreal aspectRatio = pixelAspectRatio.ratio() * displayAspectRatio.invRatio();

        QSizeF videoSizeAdjusted = QSizeF(videoSize.width() * aspectRatio, videoSize.height());
        videoSizeAdjusted.scale(targetArea.size(), aspectRatioMode);

        // the area that the original video occupies, scaled
        QRectF videoRect = QRectF(QPointF(), videoSizeAdjusted);
        videoRect.moveCenter(targetArea.center());

        if (aspectRatioMode == Qt::KeepAspectRatio) {
          videoArea = videoRect;
          sourceRect = QRectF(0, 0, 1, 1);
        } else { // Qt::KeepAspectRatioByExpanding
          videoArea = targetArea;
          sourceRect = QRectF(
              (videoArea.left() - videoRect.left()) / videoRect.width(),
              (videoArea.top() - videoRect.top()) / videoRect.height(),
              videoArea.width() / videoRect.width(),
              videoArea.height() / videoRect.height());
        }
        break;
      }
    }

    if (aspectRatioMode == Qt::IgnoreAspectRatio
        || aspectRatioMode == Qt::KeepAspectRatioByExpanding
        || videoArea == targetArea) {
        blackArea1 = blackArea2 = QRectF();
    } else {
        blackArea1 = QRectF(
            targetArea.left(),
            targetArea.top(),
            videoArea.left() == targetArea.left() ?
                targetArea.width() : videoArea.left() - targetArea.left(),
            videoArea.top() == targetArea.top() ?
                targetArea.height() : videoArea.top() - targetArea.top()
        );

        blackArea2 = QRectF(
            videoArea.right() == targetArea.right() ?
                targetArea.left() : videoArea.right(),
            videoArea.bottom() == targetArea.bottom() ?
                targetArea.top() : videoArea.bottom(),
            videoArea.right() == targetArea.right() ?
                targetArea.width() : targetArea.right() - videoArea.right(),
            videoArea.bottom() == targetArea.bottom() ?
                targetArea.height() : targetArea.bottom() - videoArea.bottom()
        );
    }
}
