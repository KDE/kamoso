import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtGStreamer 1.0
import org.kde.kamoso 3.0

ColumnLayout
{
    spacing: 1

    Label {
        font.bold: true
        text: i18n("Places")
    }
    Label {
        text: i18n("Pictures output")
    }
    RowLayout {
        Layout.fillWidth: true
        Label {
            Layout.fillWidth: true
            text: settings.saveUrl
            elide: Text.ElideLeft
        }
        Button {
            width: height
            iconName: "document-open-folder"
            onClicked: {
                dirSelector.visible = true
            }

            FileDialog {
                id: dirSelector
                title: i18n("Select a directory where to save your pictures and videos")
                selectMultiple: false
                selectExisting: true
                selectFolder: true

                onFileUrlChanged: {
                    settings.saveUrl = dirSelector.fileUrl
                }
            }
        }
    }

    Label {
        font.bold: true
        text: i18n("Camera Settings")
    }
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
    Item {
        Layout.fillHeight: true
    }
}
