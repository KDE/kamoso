import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtGStreamer 1.0
import org.kde.kquickcontrolsaddons 2.0
import org.kde.kamoso 3.0

ApplicationWindow
{
    width: 500
    height: 600
    visible: true

    ImagesView {
        id: imagesView
        mimeFilter: buttonGroup.current.stuff.mimes
        spacing: 10
        anchors {
            margins: 20
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 80
    }

    function awesomeAnimation(path) {
//         tada.x = visor.x
//         tada.y = 0
//         tada.width = visor.width
//         tada.height = visor.height
        tada.source = path
        tada.state = "go"
        tada.state = "done"
//         tada.visible = true

    }

    Connections {
        target: webcam
        onPhotoTaken: awesomeAnimation(path)
    }
    Image {
        id: tada
        z: 10
        width: 10
        height: 10
        fillMode: Image.PreserveAspectFit

        states: [
            State { name: "go"
                PropertyChanges { target: tada; x: visor.x }
                PropertyChanges { target: tada; y: visor.y }
                PropertyChanges { target: tada; width: visor.width }
                PropertyChanges { target: tada; height: visor.height }
                PropertyChanges { target: tada; opacity: 1 }
            },
            State { name: "done"
                PropertyChanges { target: tada; x: visor.width-10 }
                PropertyChanges { target: tada; y: visor.height+controls.height }
                PropertyChanges { target: tada; width: deviceSelector.height }
                PropertyChanges { target: tada; height: deviceSelector.height }
                PropertyChanges { target: tada; opacity: 0 }
            }
        ]
        transitions: [
            Transition {
                from: "go"; to: "done"
                    NumberAnimation { target: tada
                                properties: "width,height"; duration: 2500; easing.type: Easing.InOutQuad }
                    NumberAnimation { target: tada
                                properties: "x,y"; duration: 3000; easing.type: Easing.InOutQuad }
                    NumberAnimation { target: tada
                                properties: "opacity"; duration: 3000 }
            }
        ]
    }

    RowLayout {
        id: modes
        width: 100
        spacing: 10

        anchors.margins: 20
        anchors.left: parent.left
        anchors.verticalCenter: controls.verticalCenter

        ExclusiveGroup { id: buttonGroup }
        Repeater {
            model: ActionsModel { id: actions }
            delegate: Button {
                property QtObject stuff: model
                exclusiveGroup: buttonGroup
                isDefault: true
                tooltip: model.text
                checkable: true
                checked: index==0

                iconName: model.icon
            }
        }
    }

    Button {
        id: controls
        width: 100
        height: 40
        checkable: buttonGroup.current.stuff.checkable

        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: imagesView.top
        anchors.bottom: parent.bottom
        iconName: buttonGroup.current.stuff.icon

        onClicked: {
            var path = actions.trigger(buttonGroup.current.stuff.index, checked)
            if(!checkable || checked) {
//                 awesomeAnimation(path)
            }
        }
    }

    Row {
        width: 100

        anchors.margins: 20
        anchors.right: parent.right
        anchors.verticalCenter: controls.verticalCenter
        spacing: 10

        Button { width: 30; text: "a" }

        Button {
            id: settingsButton
            width: 30
            iconName: "settings"
            checkable: true
        }
    }

    Item {
        id: settingsDialog
        anchors {
            top: parent.top
            right: parent.right
            bottom: controls.top
        }
        width: settingsButton.checked ? parent.width / 3 : 0
        Behavior on width {
            PropertyAnimation { duration: 250 }
        }
        visible: width!=0
        Column {
            anchors.fill: parent
            Slider {
                id: brightnessSlider
                width: parent.width
                minimumValue: -100
                maximumValue: 100
                value: devicesModel.playingDevice.brightness

                Binding {target: devicesModel.playingDevice; property: "brightness"; value: brightnessSlider.value}
            }
            Slider {
                id: hueSlider
                width: parent.width
                minimumValue: -100
                maximumValue: 100
                value: devicesModel.playingDevice.hue

                Binding {target: devicesModel.playingDevice; property: "hue"; value: hueSlider.value}
            }
            Slider {
                id: contrastSlider
                width: parent.width
                minimumValue: 0
                maximumValue: 200
                value: devicesModel.playingDevice.contrast

                Binding {target: devicesModel.playingDevice; property: "contrast"; value: contrastSlider.value}
            }
            Slider {
                id: saturationSlider
                width: parent.width
                minimumValue: 0
                maximumValue: 200
                value: devicesModel.playingDevice.saturation

                Binding {target: devicesModel.playingDevice; property: "saturation"; value: saturationSlider.value}
            }
            Slider {
                id: gammaSlider
                width: parent.width
                minimumValue: 1
                maximumValue: 1000
                value: devicesModel.playingDevice.gamma

                Binding {target: devicesModel.playingDevice; property: "gamma"; value: gammaSlider.value}
            }
        }
    }

    Item {
        id: visor
        anchors {
            right: settingsDialog.left
            left: parent.left
            top: parent.top
            bottom: controls.top
            topMargin: 5
            bottomMargin: 5
        }

        VideoItem {
            id: video

            visible: devicesModel.count>0
            surface: videoSurface1
            anchors.fill: parent
        }

        AnimatedImage {
            anchors.fill: parent
            visible: !video.visible
            source: "http://i.imgur.com/OEiQ6k9.gif"
        }
    }

    ColumnLayout {
        id: deviceSelector
        height: 30
        spacing: 10
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left
        visible: devicesModel.count>1

        Repeater {
            model: devicesModel
            delegate: Button { width: 30; iconSource: "webcamreceive"; onClicked: devicesModel.playingDeviceUdi = udi }
        }
    }
}
