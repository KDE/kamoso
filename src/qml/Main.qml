import QtQuick 1.1
import QtGStreamer 0.10
import org.kde.plasma.core 0.1
import org.kde.kamoso 3.0
import org.kde.plasma.components 0.1

Rectangle
{
    color: "white"
    width: 500
    height: 600

    ImagesView {
        id: imagesView
        mimeFilter: modes.checkedButton.stuff.mimes
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

    ButtonRow {
        id: modes
        width: 100
        spacing: 10

        anchors.margins: 20
        anchors.left: parent.left
        anchors.verticalCenter: controls.verticalCenter

        Repeater {
            model: ActionsModel { id: actions }
            delegate: Button {
                property QtObject stuff: model

                iconSource: model.icon
            }
        }
    }

    Button {
        id: controls
        width: 100
        height: 40
        checkable: modes.checkedButton.stuff.checkable

        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: imagesView.top
        IconItem {
            anchors.centerIn: parent
            height: parent.height
            source: modes.checkedButton.stuff.icon
        }

        onClicked: {
            var path = actions.trigger(modes.checkedButton.stuff.index, checked)
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
        Button { width: 30; text: "a" }
    }

    Rectangle {
        id: settingsDialog
        visible: true
        anchors {
            top: parent.top
            right: parent.right
            bottom: controls.top
        }
        width: parent.width / 3
        Column {
            anchors.fill: parent
            Slider {
                id: brightnessSlider
                width: parent.width
                minimumValue: -100
                maximumValue: 100
                Binding {target: devicesModel.playingDevice; property: "brightness"; value: brightnessSlider.value}
            }
            Slider {
                id: hueSlider
                width: parent.width
                minimumValue: -100
                maximumValue: 100
                Binding {target: devicesModel.playingDevice; property: "hue"; value: hueSlider.value}
            }
            Slider {
                id: contrastSlider
                width: parent.width
                minimumValue: 0
                maximumValue: 200
                Binding {target: devicesModel.playingDevice; property: "contrast"; value: contrastSlider.value}
            }
            Slider {
                id: saturationSlider
                width: parent.width
                minimumValue: 0
                maximumValue: 200
                Binding {target: devicesModel.playingDevice; property: "saturation"; value: saturationSlider.value}
            }
            Slider {
                id: gammaSlider
                width: parent.width
                minimumValue: 1
                maximumValue: 1000
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
            source: "http://images3.wikia.nocookie.net/__cb20110211005027/thewargame/images/3/31/Dancing_Banana.gif"
        }
    }

    ButtonColumn {
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
