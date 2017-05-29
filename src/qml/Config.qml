import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtGStreamer 1.0
import org.kde.kamoso 3.0
import org.kde.kirigami 2.0 as Kirigami

GridView {
    id: view
    readonly property real delegateWidth: Kirigami.Units.gridUnit*4
    readonly property int columnCount: Math.floor(width/delegateWidth)
    cellWidth: width/columnCount
    cellHeight: cellWidth

    model: ListModel {
        ListElement { filters: "identity" }
        ListElement { filters: "bulge" }
        ListElement { filters: "frei0r-filter-cartoon" }
        ListElement { filters: "frei0r-filter-twolay0r" }
//                 ListElement { filters: "frei0r-filter-color-distance" }
        ListElement { filters: "dicetv" }
        ListElement { filters: "frei0r-filter-distort0r" }
        ListElement { filters: "edgetv" }
        ListElement { filters: "videoflip method=horizontal-flip" }
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

    delegate: MouseArea {
        id: delegateItem
        width:  GridView.view.cellHeight
        height: GridView.view.cellWidth

        VideoItem {
            anchors.fill: parent

            PipelineItem {
                id: pipe

                function refreshVisible() {
                    if (visible) {
                        pipe.playing = true
                    }
                    pipe.playing = false
                }
                description: "filesrc location=\"" + webcam.sampleImage + "\" ! decodebin ! imagefreeze ! videoconvert ! " + model.filters + " name=last"
                property bool dirty: false
                onDescriptionChanged: dirty = true
            }
            onVisibleChanged: {
                pipe.refreshVisible()
                if (pipe.dirty) {
                    pipe.dirty=false
                    pipe.refresh()
                }
            }
            surface: pipe.surface
        }

        onClicked: {
            devicesModel.playingDevice.filters = model.filters
        }
    }
}
