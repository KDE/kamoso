import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtGStreamer 1.0
import org.kde.kamoso 3.0

ColumnLayout
{
    spacing: 1
    clip: true

    Label {
        font.bold: true
        text: i18n("Places")
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
    Item { height: 5 }
    Label { text: i18n("Brightness:") }
    Slider {
        id: brightnessSlider
        Layout.fillWidth: true
        minimumValue: -100
        maximumValue: 100
        value: devicesModel.playingDevice.brightness

        onValueChanged: {
            devicesModel.playingDevice.brightness = value
        }
    }

    Label { text: i18n("Hue:") }
    Slider {
        id: hueSlider
        Layout.fillWidth: true
        minimumValue: -100
        maximumValue: 100
        value: devicesModel.playingDevice.hue

        onValueChanged: {
            devicesModel.playingDevice.hue = value
        }
    }

    Label { text: i18n("Contrast:") }
    Slider {
        id: contrastSlider
        Layout.fillWidth: true
        minimumValue: 0
        maximumValue: 200
        value: devicesModel.playingDevice.contrast

        onValueChanged: {
            devicesModel.playingDevice.contrast = value
        }
    }

    Label { text: i18n("Saturation:") }
    Slider {
        id: saturationSlider
        Layout.fillWidth: true
        minimumValue: 0
        maximumValue: 200
        value: devicesModel.playingDevice.saturation

        onValueChanged: {
            devicesModel.playingDevice.saturation = value
        }
    }

    Label { text: i18n("Gamma:") }
    Slider {
        id: gammaSlider
        Layout.fillWidth: true
        minimumValue: 0
        maximumValue: 999
        value: devicesModel.playingDevice.gamma

        onValueChanged: {
            //We must leave minimumValue at 0 and add it back here, otherwise we get
            //a onValueChanged when minimumValue changes and things break.
            devicesModel.playingDevice.gamma = value+1
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
