/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import org.kde.kamoso 3.0
import org.kde.kirigami 2.1 as Kirigami
import QtQuick 2.3

Item
{
    id: thumbnailRoot
    property alias path: fetcher.url
    property alias mime: fetcher.mimetype
    objectName: path
    readonly property int biggerSide: Math.max(width, height) * 2

    Kirigami.Icon {
        id: item
        anchors.centerIn: parent

        width: fetcher.previewSize.width
        height: fetcher.previewSize.height
        source: fetcher.preview

        PreviewFetcher {
            id: fetcher
            size: Qt.size(thumbnailRoot.biggerSide, thumbnailRoot.biggerSide)
        }
    }
    clip: true
}
