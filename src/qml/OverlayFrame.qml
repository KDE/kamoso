/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.2
import QtGraphicalEffects 1.0

Item {
    DropShadow {
        anchors.fill: bg
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 16
        source: bg
    }

    Rectangle
    {
        id: bg
        anchors {
            fill: parent
            margins: -10
        }
        SystemPalette {
            id: palette
        }
        color: palette.alternateBase
        radius: 5
    }
}
