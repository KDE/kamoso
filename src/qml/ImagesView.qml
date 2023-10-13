/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import org.kde.kamoso 3.0
import org.kde.purpose 1.0
import org.kde.kirigami 2.12 as Kirigami

QQC2.StackView {
    id: stack
    property string mimeFilter
    property alias nameFilter: view.nameFilter
    clip: true

    function pathOrUrl(url) {
        var urlstr = url.toString();
        if (urlstr.indexOf("file://") == 0) {
            return urlstr.substring(7);
        }
        return url;
    }

    Component {
        id: headerComponent

        ColumnLayout {
            spacing: 0

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.smallSpacing
                Layout.bottomMargin: Kirigami.Units.largeSpacing

                level: 1
                text: i18n("Share")
                elide: Text.ElideRight
            }

            Repeater {
                model: view.selection
                delegate: Kirigami.AbstractListItem {
                    id: delegate
                    Layout.minimumHeight: Kirigami.Units.gridUnit * 3
                    spacing: 0

                    RowLayout {
                        ImageThumbnail {
                            Layout.fillHeight: true
                            width: height

                            path: modelData
                        }

                        QQC2.Label {
                            Layout.fillWidth: true
                            text: modelData.substring(modelData.lastIndexOf('/')+1);
                            elide: Text.ElideLeft
                        }
                    }
                }
            }
        }
    }

    Component {
        id: chooseShareComponent
        ColumnLayout {
            property var selection
            spacing: 0

            Loader {
                Layout.fillWidth: true
                Layout.maximumHeight: item.Layout.maximumHeight

                sourceComponent: headerComponent
            }
            AlternativesView {
                id: altsView
                Layout.fillWidth: true
                Layout.fillHeight: true
                pluginType: "Export"
                inputData: {
                    "urls": view.selection,
                    "mimeType": stack.mimeFilter
                }

                verticalLayoutDirection: ListView.BottomToTop
                delegate: Kirigami.BasicListItem {
                    label: display
                    icon: model.iconName
                    onClicked: altsView.createJob(index);
                }

                onFinished: {
                    stack.replace(sharedComponent, { text: output.url })
                }
            }

            Kirigami.BasicListItem {
                label: i18n("Back")
                icon: "go-previous"
                onClicked: stack.pop()
            }
        }
    }

    Component {
        id: sharedComponent
        ColumnLayout {
            property alias text: field.text
            spacing: 0
            Loader {
                Layout.fillWidth: true
                Layout.maximumHeight: item.Layout.maximumHeight

                sourceComponent: headerComponent
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            QQC2.TextField {
                id: field
                Layout.fillWidth: true
                readOnly: true
                focus: true
                onTextChanged: {
                    selectAll();
                    copy();
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            QQC2.Label {
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                text: i18n("Media now exported")
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
            Kirigami.BasicListItem {
                label: i18n("Back")
                onClicked: {
                    stack.pop()
                }
            }
        }
    }

    Component {
        id: configureComponent

        ColumnLayout {
            property var selection
            spacing: 0

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.smallSpacing
                Layout.bottomMargin: Kirigami.Units.largeSpacing

                level: 1
                text: i18n("Configure Kamoso")
                elide: Text.ElideRight
            }

            Kirigami.FormLayout {

                Item {
                    Kirigami.FormData.isSection: true
                    Kirigami.FormData.label: i18n("Save Locations")
                }

                // Pictures location
                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.FormData.label: i18n("Pictures:")

                    QQC2.TextField {
                        readOnly: true
                        Layout.fillWidth: true
                        text: stack.pathOrUrl(config.saveUrl)
                    }

                    QQC2.Button {
                        hoverEnabled: true

                        icon.name: "document-open"

                        QQC2.ToolTip {
                            visible: parent.hovered
                            text: i18n("Choose the folder where Kamoso will save pictures")

                            // TODO: Remove this once Kamoso can depend on
                            // Frameworks 5.55, where this is fixed upstream
                            z: 999
                        }

                        onClicked: {
                            dirSelector.visible = true
                        }

                        FileDialog {
                            id: dirSelector
                            title: i18n("Choose the folder where Kamoso will save pictures")
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
                }

                // Videos location
                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.FormData.label: i18n("Videos:")

                    QQC2.TextField {
                        readOnly: true
                        Layout.fillWidth: true
                        text: stack.pathOrUrl(config.saveVideos)
                    }

                    QQC2.Button {
                        hoverEnabled: true

                        icon.name: "document-open"

                        QQC2.ToolTip {
                            visible: parent.hovered
                            text: i18n("Choose the folder where Kamoso will save videos")

                            // TODO: Remove this once Kamoso can depend on
                            // Frameworks 5.55, where this is fixed upstream
                            z: 999
                        }

                        onClicked: {
                            videoDirSelector.visible = true
                        }
                    }

                    FileDialog {
                        id: videoDirSelector
                        title: i18n("Choose the folder where Kamoso will save videos")
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
                    Kirigami.FormData.isSection: true
                    Kirigami.FormData.label: i18n("Cameras")
                    visible: camerasCombo.visible
                }

                QQC2.ComboBox {
                    id: camerasCombo
                    Layout.fillWidth: parent
                    model: devicesModel
                    textRole: "display"
                    visible: count>1
                    onActivated: {
                        devicesModel.playingDevice = devicesModel.deviceAt(index)
                    }
                }

                QQC2.CheckBox {
                    Kirigami.FormData.label: i18nc("@option:check as in, 'mirror the camera'", "Mirror camera")
                    checked: config.mirrored
                    onCheckedChanged: config.mirrored = checked
                }
            }

            // Otherwise the back button might not always be right on the bottom
            Item {
                Layout.fillHeight: true
            }

            Kirigami.BasicListItem {
                label: i18n("Back")
                icon: "go-previous"
                onClicked: stack.pop()
            }
        }
    }

    initialItem: Item {
        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            DirectoryView {
                id: view

                QQC2.ScrollBar.vertical: QQC2.ScrollBar {}
                clip: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                mimeFilter: [stack.mimeFilter]

                Kirigami.PlaceholderMessage {
                    anchors.centerIn: parent
                    width: parent.width - (4 * Kirigami.Units.largeSpacing)

                    visible: view.count == 0

                    text: xi18nc("@info", "There are no images in <filename>%1</filename>", stack.pathOrUrl(config.saveUrl))
                }
            }

            Kirigami.Separator {
                Layout.fillWidth: true
            }

            Kirigami.BasicListItem {
                enabled: view.selection.length > 0

                icon: "document-share"
                label: enabled? i18np("Share %1 Item...", "Share %1 Items...", view.selection.length) : i18n("Share Item...")
                onClicked: stack.push(chooseShareComponent, { selection: view.selection })
            }

            Kirigami.BasicListItem {
                enabled: view.selection.length > 0

                icon: "user-trash"
                label: enabled ? i18np("Move %1 Item to Trash", "Move %1 Items to Trash", view.selection.length) : i18n("Move Item to Trash")

                onClicked: {
                    webcam.trashFiles(view.selection);
                }
            }

            Kirigami.BasicListItem {
                icon: "folder-open"
                label: i18n("Open Pictures Folder")

                onClicked: Qt.openUrlExternally(config.saveUrl)
            }

            Kirigami.BasicListItem {
                icon: "configure"
                label: i18n("Configure Kamoso...")
                onClicked: {
                    stack.push(configureComponent, { selection: view.selection })
                }
            }
        }
    }
}
