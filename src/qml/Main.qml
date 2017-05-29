import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtGStreamer 1.0
import org.kde.kirigami 2.0 as Kirigami
import org.kde.kamoso 3.0

Kirigami.ApplicationWindow
{
    id: root
    width: 700
    height: width*3/4
    visible: true
    title: i18n("Kamoso")
    header: Item {}

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
                PropertyChanges { target: tada; x: 0 }
                PropertyChanges { target: tada; y: root.height }
                PropertyChanges { target: tada; width: deviceSelector.height }
                PropertyChanges { target: tada; height: deviceSelector.height }
                PropertyChanges { target: tada; opacity: 0.5 }
            }
        ]
        transitions: [
            Transition {
                from: "go"; to: "done"
                    NumberAnimation { target: tada
                                properties: "width,height"; duration: 700; easing.type: Easing.InCubic }
                    NumberAnimation { target: tada
                                properties: "x,y"; duration: 700; easing.type: Easing.InCubic }
                    NumberAnimation { target: tada
                                properties: "opacity"; duration: 300 }
            }
        ]
    }

    Mode {
        id: photoMode
        mimes: "image/jpeg"
        checkable: false
        iconName: "shoot"
        text: i18n("Shoot")
        nameFilter: "picture_*"

        onTriggered: {
            whites.showAll()
            webcam.takePhoto()
        }

        Connections {
            target: webcam
            onPhotoTaken: awesomeAnimation(path)
        }
    }
    Mode {
        id: burstMode
        mimes: "image/jpeg"
        checkable: true
        iconName: "burst"
        text: i18n("Burst")
        property int photosTaken: 0
        modeInfo: (photosTaken>0 ? i18np("1 photo", "%1 photos", photosTaken) : "") + (checked? "..." : "")
        nameFilter: "picture_*"
        onCheckedChanged: if (checked) {
            photosTaken = 0
        }

        readonly property var smth: Timer {
            id: burstTimer
            running: burstMode.checked
            interval: 1000
            repeat: true
            onTriggered: {
                whites.showAll()
                webcam.takePhoto()
                burstMode.photosTaken++;
            }
        }
    }
    Mode {
        id: videoMode
        mimes: "video/x-matroska"
        checkable: true
        iconName: "record"
        text: i18n("Record")
        modeInfo: webcam.recordingTime
        nameFilter: "video_*"

        onCheckedChanged: {
            webcam.isRecording = checked;
        }
    }

    contextDrawer: Kirigami.OverlayDrawer {
        edge: Qt.RightEdge
        drawerOpen: false
        handleVisible: true
        modal: true

        leftPadding: 0
        topPadding: 0
        rightPadding: 0
        bottomPadding: 0

        contentItem: ImagesView {
            id: view
            implicitWidth: Kirigami.Units.gridUnit * 20
            mimeFilter: root.pageStack.currentItem.actions.main.mimes
            nameFilter: root.pageStack.currentItem.actions.main.nameFilter
        }
    }

    globalDrawer: Kirigami.OverlayDrawer {
        edge: Qt.LeftEdge
        drawerOpen: false
        handleVisible: true
        modal: true
        width: Kirigami.Units.gridUnit * 20

        leftPadding: 0
        topPadding: 0
        rightPadding: 0
        bottomPadding: 0

        contentItem: Config {
            id: configView
            header: Image {
                    fillMode: Image.PreserveAspectCrop
                    width: configView.width
                    height: Kirigami.Units.gridUnit * 10
                    source: "https://images.unsplash.com/photo-1484781663516-4c4ca4b04a13?dpr=1&auto=format&fit=crop&w=1500&h=1021&q=80&cs=tinysrgb"
                    smooth: true

                    Kirigami.Heading {
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                            margins: Kirigami.Units.smallSpacing * 2
                        }
                        level: 1
                        color: "white"
                        elide: Text.ElideRight
                        text: i18n("Kamoso Gallery")
                    }
                }
        }
    }

    pageStack.initialPage: Kirigami.Page {
        id: visor
        bottomPadding: 0
        topPadding: 0
        rightPadding: 0
        leftPadding: 0

        state: "shoot"
        states: [
            State {
                name: "shoot"
                PropertyChanges {
                    target: visor
                    actions {
                        left: videoMode.adoptAction
                        main: photoMode
                        right: burstMode.adoptAction
                    }
                }
            },
            State {
                name: "record"
                PropertyChanges {
                    target: visor
                    actions {
                        left: photoMode.adoptAction
                        main: videoMode
                        right: burstMode.adoptAction
                    }
                }
            },

            State {
                name: "burst"
                PropertyChanges {
                    target: visor
                    actions {
                        left: videoMode.adoptAction
                        main: burstMode
                        right: photoMode.adoptAction
                    }
                }
            }
        ]

        Rectangle {
            anchors.fill: parent
            color: "black"
            z: -1
        }

        ColumnLayout {
            id: deviceSelector
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

        VideoItem {
            id: video

            visible: devicesModel.count>0
            surface: videoSurface1
            anchors.fill: parent
        }

        AnimatedImage {
            anchors.fill: video
            visible: !video.visible
            source: visible ? "http://i.imgur.com/OEiQ6k9.gif" : ""
        }

        Text {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                margins: 20
            }

            text: root.pageStack.currentItem.actions.main.modeInfo
            color: "white"
            styleColor: "black"
            font.pointSize: 20

            style: Text.Outline
        }
    }
}
