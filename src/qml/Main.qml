/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.5
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import org.kde.kirigami 2.9 as Kirigami
import org.kde.kamoso 3.0
import org.freedesktop.gstreamer.Qt6GLVideoItem

Kirigami.ApplicationWindow
{
    id: root
    visible: true
    title: i18n("Kamoso")
    pageStack.globalToolBar.toolbarActionAlignment: Qt.AlignCenter
    minimumWidth: 320
    minimumHeight: 240 + pageStack.globalToolBar.height
    Component.onCompleted: {
        width = 700
        height = width*3/4
    }

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
        function onError(error) { showPassiveNotification(error) }
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
            },
            State { name: "done"
                PropertyChanges { target: tada; x: root.width }
                PropertyChanges { target: tada; y: root.height }
                PropertyChanges { target: tada; width: Kirigami.Units.gridUnit }
                PropertyChanges { target: tada; height: Kirigami.Units.gridUnit }
            }
        ]
        transitions: [
            Transition {
                from: "go"; to: "done"
                    NumberAnimation { target: tada
                                properties: "width,height"; duration: 700; easing.type: Easing.InCubic }
                    NumberAnimation { target: tada
                                properties: "x,y"; duration: 700; easing.type: Easing.InCubic }
            }
        ]
    }

    property Mode lastMode: photoMode
    Mode {
        id: photoMode
        mimes: "image/jpeg"
        checkable: false
        icon.name: "camera-photo-symbolic"
        text: i18n("Take a Picture")
        nameFilter: "picture_*"
        enabled: devicesModel.playingDevice

        onTriggered: {
            webcam.takePhoto()
            lastMode = photoMode
        }

        Connections {
            target: webcam
            function onPhotoTaken(path) { awesomeAnimation(path) }
        }
    }

    Binding {
        target: webcam
        property: "mirrored"
        value: config.mirrored
    }

    Mode {
        id: burstMode
        mimes: "image/jpeg"
        checkable: true
        icon.name: checked ? "media-playback-stop" : "burst"
        text: checked? i18n("End Burst") : i18n("Capture a Burst")
        property int photosTaken: 0
        modeInfo:  photosTaken > 0 ? i18np("1 photo taken", "%1 photos taken", photosTaken) : ""
        nameFilter: "picture_*"
        enabled: devicesModel.playingDevice && !videoMode.checked
        onCheckedChanged: if (checked) {
            photosTaken = 0
            lastMode = burstMode
        }

        readonly property var smth: Timer {
            id: burstTimer
            running: burstMode.checked
            interval: 2500
            repeat: true
            onTriggered: {
                webcam.takePhoto()
                burstMode.photosTaken++;
            }
        }
    }
    Mode {
        id: videoMode
        mimes: "video/x-matroska"
        checkable: true
        icon.name: checked ? "media-playback-stop" : "camera-video-symbolic"
        text: checked? i18n("Stop Recording") : i18n("Record a Video")
        modeInfo: webcam.recordingTime
        nameFilter: "video_*"
        enabled: devicesModel.playingDevice && !burstMode.checked

        onCheckedChanged: checked => {
            webcam.isRecording = checked;
            lastMode = videoMode
        }
    }

    contextDrawer: Kirigami.OverlayDrawer {
        edge: Qt.RightEdge
        drawerOpen: false
        handleVisible: true
        handleClosedIcon.source: "configure"
        modal: true

        leftPadding: 0
        topPadding: 0
        rightPadding: 0
        bottomPadding: 0

        // Should probably make it switch modes without having to trigger them, or maybe just show all modes at once?
        contentItem: ImagesView {
            implicitWidth: Kirigami.Units.gridUnit * 20
            mimeFilter: lastMode.mimes
            nameFilter: lastMode.nameFilter
        }
    }

    globalDrawer: Kirigami.OverlayDrawer {
        edge: Qt.LeftEdge
        drawerOpen: false
        handleVisible: true
        handleClosedIcon.source: "special-effects-symbolic"
        handleOpenIcon.source: "view-left-close"
        modal: true
        width: Kirigami.Units.gridUnit * 20

        leftPadding: Kirigami.Units.smallSpacing
        topPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
        bottomPadding: Kirigami.Units.smallSpacing

        contentItem: Config {
            id: configView

            QQC2.ScrollBar.vertical: QQC2.ScrollBar {}

            header: QQC2.Control {
                height: effectsGalleryHeading.height + Kirigami.Units.largeSpacing
                Kirigami.Heading {
                    id: effectsGalleryHeading
                    level: 1
                    color: Kirigami.Theme.textColor
                    elide: Text.ElideRight
                    text: i18n("Effects Gallery")
                }
            }
        }
    }

    Shortcut {
        sequence: "Return"
        onActivated: lastMode.triggered(null)
    }

    pageStack.initialPage: Kirigami.Page {
        id: visor
        bottomPadding: 0
        topPadding: 0
        rightPadding: 0
        leftPadding: 0

        actions: [ videoMode, photoMode, burstMode ]

        Rectangle {
            anchors.fill: parent
            color: "black"
            z: -1
        }

        GstGLQt6VideoItem {
            acceptEvents: false
            objectName: "sink"
            anchors.fill: parent
        }

        Text {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                margins: 20
            }

            text: {
                if (!devicesModel.playingDevice) {
                    return i18n("No device found");
                }
                if (videoMode.checked) {
                    return videoMode.modeInfo;
                }
                if (burstMode.checked) {
                    return burstMode.modeInfo;
                }
                return "";
            }
            color: "white"
            styleColor: "black"
            font.pointSize: 20

            style: Text.Outline
        }
    }
}
