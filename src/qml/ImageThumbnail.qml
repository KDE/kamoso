/************************************************************************************
 * Copyright (C) 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>                *
 *                                                                                  *
 * This program is free software; you can redistribute it and/or                    *
 * modify it under the terms of the GNU General Public License                      *
 * as published by the Free Software Foundation; either version 2                   *
 * of the License, or (at your option) any later version.                           *
 *                                                                                  *
 * This program is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 * GNU General Public License for more details.                                     *
 *                                                                                  *
 * You should have received a copy of the GNU General Public License                *
 * along with this program; if not, write to the Free Software                      *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 ************************************************************************************/

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
