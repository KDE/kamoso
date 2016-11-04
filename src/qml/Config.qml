import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtMultimedia 5.6
import org.kde.kamoso 3.0

ColumnLayout
{
    spacing: 1
    clip: true

    Label {
        font.bold: true
        text: i18n("Places")
    }

    property var cameraInstance: null

    function pathOrUrl(url) {
        var urlstr = url.toString();
        if (urlstr.indexOf("file://") == 0) {
            return urlstr.substring(7);
        }
        return url;
    }

    Button {
        Layout.fillWidth: true

        iconName: "document-open-folder"
        text: i18n("Photos: %1", pathOrUrl(config.saveUrl))
        onClicked: {
            dirSelector.visible = true
        }

        FileDialog {
            id: dirSelector
            title: i18n("Select a directory where to save your pictures and videos")
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

        iconName: "document-open-folder"
        text: i18n("Videos: %1", pathOrUrl(config.saveVideos))
        onClicked: {
            videoDirSelector.visible = true
        }

        FileDialog {
            id: videoDirSelector
            title: i18n("Select a directory where to save your pictures and videos")
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

    Item { height: 15 }

    Label {
        font.bold: true
        text: i18n("Camera Settings")
    }
    CheckBox {
        text: i18n("Use flash")
        checked: config.useFlash
        onCheckedChanged: config.useFlash = checked
    }
    CheckBox {
        text: i18n("On screen information")
        checked: config.showOsd
        onCheckedChanged: config.showOsd = checked
    }
    Item { height: 5 }
    Label { text: i18n("Brightness:") }
    Slider {
        id: brightnessSlider
        Layout.fillWidth: true
        minimumValue: -1
        maximumValue: 1
        value: cameraInstance.imageProcessing.brightness

        onValueChanged: {
            cameraInstance.imageProcessing.brightness = value
        }
    }

    Label { text: i18n("Contrast:") }
    Slider {
        id: contrastSlider
        Layout.fillWidth: true
        minimumValue: -1
        maximumValue: 1
        value: cameraInstance.imageProcessing.contrast

        onValueChanged: {
            cameraInstance.imageProcessing.contrast = value
        }
    }

    Label { text: i18n("Saturation:") }
    Slider {
        id: saturationSlider
        Layout.fillWidth: true
        minimumValue: -1
        maximumValue: 1
        value: cameraInstance.imageProcessing.saturation

        onValueChanged: {
            cameraInstance.imageProcessing.saturation = value
        }
    }


    Label { text: i18n("Color Filter:") }
    ComboBox {
        id: colorCombo
        Layout.fillWidth: true
        model: ListModel { id: theModel }
        Component.onCompleted: {
            theModel.append({display: i18n("None"), value: CameraImageProcessing.ColorFilterNone })
            theModel.append({display: i18n("Grayscale"), value: CameraImageProcessing.ColorFilterGrayscale })
            theModel.append({display: i18n("Negative"), value: CameraImageProcessing.ColorFilterNegative })
            theModel.append({display: i18n("Solarize"), value: CameraImageProcessing.ColorFilterSolarize })
            theModel.append({display: i18n("Sepia"), value: CameraImageProcessing.ColorFilterSepia })
            theModel.append({display: i18n("Posterize"), value: CameraImageProcessing.ColorFilterPosterize })
            theModel.append({display: i18n("Whiteboard"), value: CameraImageProcessing.ColorFilterWhiteboard })
            theModel.append({display: i18n("Blackboard"), value: CameraImageProcessing.ColorFilterBlackboard })
            theModel.append({display: i18n("Aqua"), value: CameraImageProcessing.ColorFilterAqua })
            theModel.append({display: i18n("Vendor"), value: CameraImageProcessing.ColorFilterVendor })
        }
        textRole: "display"

        onCurrentIndexChanged: {
            var item = theModel.get(currentIndex)
            if (!item)
                return
            cameraInstance.imageProcessing.colorFilter = item.value
//             console.log("fuuu", cameraInstance.imageProcessing.colorFilter, JSON.stringify(item))
        }
    }

    Button {
        anchors.right: parent.right
        text: i18n("Reset")
        onClicked: {
            webcam.resetDeviceSettings()
        }
    }
    Item {
        Layout.fillHeight: true
    }
}
