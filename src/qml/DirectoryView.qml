/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 2.4
import org.kde.kirigami 2.0 as Kirigami
import org.kde.kamoso 3.0

GridView
{
    id: view
    readonly property real delegateWidth: Kirigami.Units.gridUnit*4
    readonly property int columnCount: Math.floor(width/delegateWidth)

    property alias mimeFilter: model.mimeFilter
    property alias nameFilter: model.nameFilter

    property var selection: []

    cellWidth: width/columnCount
    cellHeight: cellWidth

    model: DirModel {
        id: model
        url: config.saveUrl
    }
    onCountChanged: {
        view.selection = []
    }

    delegate: MouseArea {
        id: delegateItem
        width:  GridView.view.cellHeight
        height: GridView.view.cellWidth
        acceptedButtons: Qt.AllButtons

        onClicked: {
            var url = path.toString();
            var idx = view.selection.indexOf(url);
            if (idx < 0) {
                view.selection.push(url)
            } else {
                view.selection.splice(idx, 1)
            }
            view.selectionChanged(view.selection);
        }

        Rectangle {
            anchors {
                margins: -1
                fill: parent
            }
            visible: view.selection.indexOf(path.toString())>=0

            SystemPalette {
                id: pal
            }
            opacity: 0.6
            color: "transparent"
            border.color: pal.highlight
            border.width: Kirigami.Units.smallSpacing * 2
            z: 1
        }

        ImageThumbnail {
            anchors {
                fill: parent
                leftMargin: 1
            }
            path: model.path
            mime: model.mime
        }

        CheckBox {
            anchors {
                right: parent.right
                rightMargin: -width/3
            }
            checkState: Qt.Checked
            enabled: false
            visible: view.selection.indexOf(path.toString())>=0
        }
    }
}
