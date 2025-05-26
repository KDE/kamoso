/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kamoso
import org.kde.kirigami as Kirigami
import org.freedesktop.gstreamer.Qt6GLVideoItem

GridView {
    id: view
    readonly property int columnCount: 3
    cellWidth: Math.floor(width / columnCount)
    cellHeight: cellWidth * 3/4


    model: ListModel {
        ListElement { filters: "identity" }
        ListElement { filters: "bulge" }
        ListElement { filters: "frei0r-filter-cartoon" }
        ListElement { filters: "frei0r-filter-twolay0r" }
//                 ListElement { filters: "frei0r-filter-color-distance" }
        ListElement { filters: "dicetv" }
        ListElement { filters: "frei0r-filter-distort0r" }
        ListElement { filters: "edgetv" }
        ListElement { filters: "coloreffects preset=heat" }
        ListElement { filters: "videobalance saturation=0 ! agingtv" }
        ListElement { filters: "videobalance saturation=1.5 hue=-0.5" }
//                 ListElement { filters: "frei0r-filter-invert0r" }
        ListElement { filters: "kaleidoscope" }
        ListElement { filters: "videobalance saturation=1.5 hue=+0.5" }
        ListElement { filters: "mirror" }
        ListElement { filters: "videobalance saturation=0" }
        ListElement { filters: "optv" }
        ListElement { filters: "pinch" }
        ListElement { filters: "quarktv" }
        ListElement { filters: "radioactv" }
        ListElement { filters: "revtv" }
        ListElement { filters: "rippletv" }
        ListElement { filters: "videobalance saturation=2" }
        ListElement { filters: "coloreffects preset=sepia" }
        ListElement { filters: "shagadelictv" }
//                 ListElement { filters: "frei0r-filter-sobel" }
        ListElement { filters: "square" }
        ListElement { filters: "streaktv" }
        ListElement { filters: "stretch" }
        ListElement { filters: "frei0r-filter-delay0r delaytime=1" }
        ListElement { filters: "twirl" }
        ListElement { filters: "vertigotv" }
        ListElement { filters: "warptv" }
        ListElement { filters: "coloreffects preset=xray" }
    }

    readonly property string sampleImage: view.visible && WebcamControl.currentDevice.length > 0 && sampleImage !== Kamoso.sampleImage ? Kamoso.sampleImage : ""

    delegate: Rectangle {
        readonly property int borderWidth: 2
        id: delegateItem

        width: Math.floor(view.width / columnCount) - Kirigami.Units.smallSpacing
        height: width * 3/4

        color: DeviceManager.playingDevice.filters === model.filters ? Kirigami.Theme.highlightColor : Kirigami.Theme.textColor

        MouseArea {
            anchors.centerIn: parent

            width:  delegateItem.width - (borderWidth * 2)
            height: delegateItem.height - (borderWidth * 2)

            GstGLQt6VideoItem {
                anchors.fill: parent
                acceptEvents: false

                PipelineItem {
                    playing: false
                    onFailed: {
                        delegateItem.visible = false
                        view.model.remove(index)
                    }

                    description: view.sampleImage.length === 0 ? "" : "filesrc location=\"" + view.sampleImage + "\" ! decodebin ! imagefreeze ! videoconvert ! " + model.filters + " name=last"
                }
            }

            onClicked: {
                if (DeviceManager.playingDevice.filters === model.filters)
                    DeviceManager.playingDevice.filters = ""
                else
                    DeviceManager.playingDevice.filters = model.filters
            }
        }
    }
}
