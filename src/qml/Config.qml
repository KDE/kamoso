import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtGStreamer 1.0
import org.kde.kamoso 3.0
import org.kde.kirigami 2.0 as Kirigami

ColumnLayout
{
    spacing: 1
    clip: true

    ScrollView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        GridView {
            readonly property real delegateWidth: Kirigami.Units.gridUnit*4
            readonly property int columnCount: Math.floor(width/delegateWidth)
            cellWidth: width/columnCount
            cellHeight: cellWidth

            header: Label {
                font.bold: true
                text: i18n("Effects")
            }

            model: ListModel {
                ListElement { filters: "" }
                ListElement { filters: "bulge" }
                ListElement { filters: "frei0r-filter-cartoon" }
                ListElement { filters: "frei0r-filter-twolay0r" }
                ListElement { filters: "frei0r-filter-color-distance" }
                ListElement { filters: "dicetv" }
                ListElement { filters: "frei0r-filter-distort0r" }
                ListElement { filters: "edgetv" }
                ListElement { filters: "videoflip method=horizontal-flip" }
                ListElement { filters: "coloreffects preset=heat" }
                ListElement { filters: "videobalance saturation=0 ! agingtv" }
                ListElement { filters: "videobalance saturation=1.5 hue=-0.5" }
                ListElement { filters: "frei0r-filter-invert0r" }
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
                ListElement { filters: "frei0r-filter-sobel" }
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
                width:  GridView.view.cellHeight-1
                height: GridView.view.cellWidth-1

                Text {
                    text: model.index
                }

                Rectangle {
                    color: "green"
                    anchors.fill: parent
                }

                onClicked: {
                    devicesModel.playingDevice.filters = model.filters
                }
            }
        }
    }

    Item { height: 15 }

    Label {
        font.bold: true
        text: i18n("Save to...")
    }

    function pathOrUrl(url) {
        var urlstr = url.toString();
        if (urlstr.indexOf("file://") == 0) {
            return urlstr.substring(7);
        }
        return url;
    }

    Button {
        Layout.fillWidth: true

        iconName: "folder-pictures"
        text: i18n("%1", pathOrUrl(config.saveUrl))
        onClicked: {
            dirSelector.visible = true
        }

        FileDialog {
            id: dirSelector
            title: i18n("Select a directory where to save your pictures")
            folder: config.saveUrl
            selectMultiple: false
            selectExisting: true
            selectFolder: true

            onFileUrlChanged: {
                config.saveUrl = dirSelector.fileUrl
                config.save()
            }
        }
    }
    Button {
        Layout.fillWidth: true

        iconName: "folder-videos"
        text: i18n("%1", pathOrUrl(config.saveVideos))
        onClicked: {
            videoDirSelector.visible = true
        }

        FileDialog {
            id: videoDirSelector
            title: i18n("Select a directory where to save your videos")
            folder: config.saveVideos
            selectMultiple: false
            selectExisting: true
            selectFolder: true

            onFileUrlChanged: {
                config.saveVideos = videoDirSelector.fileUrl
                config.save()
            }
        }
    }
    Item {
        Layout.fillHeight: true
    }
}
