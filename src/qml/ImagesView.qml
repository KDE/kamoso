import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import org.kde.kamoso 3.0
import org.kde.purpose 1.0
import org.kde.kirigami 2.0 as Kirigami

StackView {
    id: stack
    property string mimeFilter
    property alias nameFilter: view.nameFilter
    property Component header
    clip: true

    Component {
        id: chooseShareComponent
        Item {
            id: menu
            property var selection
            AlternativesView {
                id: altsView
                anchors.fill: parent
                pluginType: "Export"
                inputData: {
                    "urls": view.selection,
                    "mimeType": stack.mimeFilter
                }

                delegate: Kirigami.BasicListItem {
                    label: display
                    onClicked: altsView.createJob(index);
                }

                header: ColumnLayout {
                    width: altsView.width
                    spacing: 0
                    Image {
                        fillMode: Image.PreserveAspectCrop
                        Layout.fillWidth: true
                        Layout.maximumHeight: Kirigami.Units.gridUnit * 10
                        source: "http://unsplash.com/photos/RwjciZ9JEfg/download?force=true"
                    }

                    Repeater {
                        model: view.selection
                        delegate: Kirigami.AbstractListItem {
                            id: delegate
                            Layout.minimumHeight: Kirigami.Units.gridUnit * 3
                            spacing: 0

                            RowLayout {
                                ImageThumbnail {
                                    fetchWidth: delegate.Layout.minimumHeight
                                    fetchHeight: fetchWidth

                                    Layout.fillHeight: true
                                    width: fetchWidth
                                    path: modelData
                                }

                                Kirigami.Label {
                                    text: modelData
                                }
                            }
                        }
                    }
                }

                footer: Kirigami.BasicListItem {
                    label: i18n("Back")
                    onClicked: stack.pop()
                }

                onFinished: stack.replace({
                    item: sharedComponent,
                    properties: { text: output.url },
                    replace: true
                })
            }
        }
    }

    Component {
        id: sharedComponent
        ColumnLayout {
            property alias text: field.text
            spacing: 0
            TextField {
                id: field
                Layout.fillWidth: true
                readOnly: true
                focus: true
                onTextChanged: {
                    selectAll();
                    copy();
                }
            }
            Kirigami.Label {
                Layout.fillHeight: true
                text: i18n("The media is now shared")
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

    initialItem: Item {
        ColumnLayout {
            anchors.fill: parent
            DirectoryView {
                id: view
                header: Image {
                    fillMode: Image.PreserveAspectCrop
                    width: view.width
                    height: Kirigami.Units.gridUnit * 10
                    source: "https://images.unsplash.com/photo-1481933236927-d92e97e3194c?ixlib=rb-0.3.5&q=80&fm=jpg&crop=entropy&cs=tinysrgb&s=25a62a567881e623a7a9cd5819ed0910"
                }

                Layout.fillWidth: true
                Layout.fillHeight: true
                mimeFilter: [stack.mimeFilter]
            }
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                Kirigami.Separator {
                    Layout.fillWidth: true
                }

                Kirigami.BasicListItem {
                    icon: "user-trash"
                    label: i18n("Move to trash... (%1)", view.selection.length)
                    visible: view.selection.length>0
                    onClicked: {
                        trashDialog.visible = true
                    }
                    readonly property var p0: Dialog {
                        id: trashDialog
                        title: i18n("Move to trash...")

                        Label {
                            text: i18np("Are you sure you want to remove %1 file?", "Are you sure you want to remove %1 files?", view.selection.length)
                        }

                        standardButtons: StandardButton.Ok | StandardButton.Cancel
                        onAccepted: {
                            console.log("Trash, FFS!!", view.selection);
                            webcam.trashFiles(view.selection);
                        }
                        onVisibleChanged: if (!visible) {
                            view.selection = []
                        }
                    }
                }
                Kirigami.BasicListItem {
                    icon: "document-share"
                    label: i18n("Share... (%1)", view.selection.length)
                    onClicked: stack.push({
                        item: chooseShareComponent,
                        properties: { selection: view.selection }
                    })
                    visible: view.selection.length>0
                }
                Kirigami.BasicListItem {
                    icon: "folder-open"
                    label: i18n("Open Folder...")
                    onClicked: Qt.openUrlExternally(config.saveUrl)
                }
            }
        }
    }
}
