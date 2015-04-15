import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtGStreamer 1.0
import org.kde.kquickcontrolsaddons 2.0
import org.kde.kamoso 3.0

ApplicationWindow
{
    width: 700
    height: 450
    visible: true

    function awesomeAnimation(path) {
//         tada.x = visor.x
//         tada.y = 0
//         tada.width = visor.width
//         tada.height = visor.height
        tada.source = "file://"+path
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

    Item {
        anchors {
            left: parent.left
            right: controls.left
            top: controls.top
            bottom: parent.bottom
        }
        RowLayout {
            anchors.centerIn: parent
            spacing: 0

            ExclusiveGroup { id: buttonGroup }
            Repeater {
                model: ActionsModel { id: actions }
                delegate: Button {
                    property QtObject stuff: model
                    exclusiveGroup: buttonGroup
                    isDefault: true
                    tooltip: i18n("Switch to '%1' mode", model.text)
                    checkable: true
                    checked: index==0

                    iconName: model.icon
                }
            }
        }
    }

    Button {
        id: controls
        width: 100
        height: 40
        checkable: buttonGroup.current.stuff.checkable
        tooltip: buttonGroup.current.stuff.text

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        QIconItem {
            anchors {
                centerIn: parent
            }
            height: parent.height*0.75
            width: height
            icon: buttonGroup.current.stuff.icon
        }

        onClicked: {
            var path = actions.trigger(buttonGroup.current.stuff.index, checked)
            if(!checkable || checked) {
//                 awesomeAnimation(path)
            }
        }
    }

    Item {
        anchors {
            top: controls.top
            left: controls.right
            right: parent.right
            bottom: parent.bottom
        }
        RowLayout {
            anchors.centerIn: parent
            spacing: 0
            Button {
                id: galleryButton
                checkable: true
                checked: false
                iconName: "folder-images"
                tooltip: i18n("Show gallery...")
            }

            Button {
                id: settingsButton
                width: 30
                iconName: "configure"
                checkable: true
                tooltip: i18n("Show settings...")
            }
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
        Config {
            anchors {
                fill: parent
                margins: 5
            }
        }
    }

    Rectangle {
        id: visor
        color: "black"

        anchors {
            right: settingsDialog.left
            left: parent.left
            top: parent.top
            bottom: controls.top
        }

        VideoItem {
            id: video

            visible: devicesModel.count>0
            surface: videoSurface1
            anchors.fill: parent
        }

        AnimatedImage {
            anchors.fill: video
            visible: !video.visible
            source: "http://i.imgur.com/OEiQ6k9.gif"
        }

        OverlayFrame {
            width: visor.width*2/3
            height: visor.height*2/3
            visible: galleryButton.checked
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 30
            }

            ImagesView {
                id: imagesView
                anchors.fill: parent
                mimeFilter: buttonGroup.current.stuff.mimes
            }
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
            delegate: Button {
                width: 30
                iconName: "camera-web"
                tooltip: display
                onClicked: devicesModel.playingDeviceUdi = udi
            }
        }
    }
}
