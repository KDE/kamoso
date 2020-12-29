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

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import KamosoQtGStreamer 1.0
import org.kde.kamoso 3.0
import org.kde.kirigami 2.0 as Kirigami

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


    property string sampleImage: ""
    onVisibleChanged: if (view.visible) {
        sampleImage = webcam.sampleImage
    }

    delegate: Rectangle {
        readonly property int borderWidth: 2
        id: delegateItem

        width: Math.floor(view.width / columnCount) - Kirigami.Units.smallSpacing
        height: width * 3/4

        color: devicesModel.playingDevice.filters === model.filters?Kirigami.Theme.selectionFocusColor : Kirigami.Theme.textColor

        MouseArea {
            anchors.centerIn: parent

            width:  delegateItem.width - (borderWidth * 2)
            height: delegateItem.height - (borderWidth * 2)

            VideoItem {
                anchors.fill: parent

                PipelineItem {
                    id: pipe

                    playing: false
                    onFailed: {
                        delegateItem.visible = false
                        view.model.remove(index)
                    }

                    description: view.sampleImage.length === 0 ? "" : "filesrc location=\"" + view.sampleImage + "\" ! decodebin ! imagefreeze ! videoconvert ! " + model.filters + " name=last"
                }
                surface: pipe.surface
            }

            onClicked: {
                if (devicesModel.playingDevice.filters === model.filters)
                    devicesModel.playingDevice.filters = ""
                else
                    devicesModel.playingDevice.filters = model.filters
            }
        }
    }
}
