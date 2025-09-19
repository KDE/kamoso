/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQml
import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kamoso
import org.kde.purpose
import org.kde.kirigami as Kirigami

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
                delegate: QQC2.ItemDelegate {
                    height: Kirigami.Units.gridUnit * 3

                    contentItem: RowLayout {
                        spacing: Kirigami.Units.smallSpacing
                        ImageThumbnail {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height

                            path: modelData
                        }

                        QQC2.Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
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
                delegate: QQC2.ItemDelegate {
                    text: display
                    icon.name: model.iconName
                    onClicked: altsView.createJob(index);
                }

                onFinished: {
                    stack.replace(sharedComponent, { text: output.url })
                }
            }

            QQC2.ItemDelegate {
                text: i18n("Back")
                icon.name: "go-previous"
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
            QQC2.ItemDelegate {
                text: i18n("Back")
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
                    Kirigami.FormData.label: i18n("Cameras")
                    visible: camerasCombo.visible
                }

                QQC2.ComboBox {
                    id: camerasCombo
                    Layout.fillWidth: parent
                    model: DeviceManager
                    textRole: "display"
                    visible: count>1
                    onActivated: (row) => {
                        DeviceManager.playingDevice = DeviceManager.deviceAt(row);
                    }
                }

                QQC2.CheckBox {
                    text: i18nc("@option:check as in, 'mirror the camera'", "Mirror camera")
                    checked: Settings.mirrored
                    onCheckedChanged: Settings.mirrored = checked
                }
            }

            // Otherwise the back button might not always be right on the bottom
            Item {
                Layout.fillHeight: true
            }

            QQC2.ItemDelegate {
                text: i18n("Back")
                icon.name: "go-previous"
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

                    text: xi18nc("@info", "There are no images in <filename>%1</filename>", stack.pathOrUrl(Settings.saveUrl))
                }
            }

            Kirigami.Separator {
                Layout.fillWidth: true
            }

            QQC2.ItemDelegate {
                Layout.fillWidth: true

                enabled: view.selection.length > 0

                icon.name: "document-share"
                text: enabled? i18np("Share %1 Item...", "Share %1 Items...", view.selection.length) : i18n("Share Item...")
                onClicked: stack.push(chooseShareComponent, { selection: view.selection })
            }

            QQC2.ItemDelegate {
                Layout.fillWidth: true

                enabled: view.selection.length > 0

                icon.name: "user-trash"
                text: enabled ? i18np("Move %1 Item to Trash", "Move %1 Items to Trash", view.selection.length) : i18n("Move Item to Trash")

                onClicked: {
                    Kamoso.trashFiles(view.selection);
                }
            }

            QQC2.ItemDelegate {
                Layout.fillWidth: true

                icon.name: "folder-open"
                text: i18n("Open Pictures Folder")

                onClicked: Qt.openUrlExternally(Settings.saveUrl)
            }

            QQC2.ItemDelegate {
                Layout.fillWidth: true

                icon.name: "configure"
                text: i18n("Configure Kamoso...")
                onClicked: {
                    stack.push(configureComponent, { selection: view.selection })
                }
            }
        }
    }
}
