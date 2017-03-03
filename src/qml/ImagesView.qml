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
    clip: true

    Component {
        id: headerComponent
        ColumnLayout {
            spacing: 0
            Layout.maximumHeight: Kirigami.Units.gridUnit * 10
            Image {
                fillMode: Image.PreserveAspectCrop
                Layout.fillWidth: true
                Layout.fillHeight: true
                source: "https://images.unsplash.com/photo-1486893732792-ab0085cb2d43?dpr=1&auto=format&fit=crop&w=767&h=462&q=80&cs=tinysrgb&crop="

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
                    text: i18n("Share...")
                }
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
                            Layout.fillWidth: true
                            text: modelData
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
            id: menu
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

                delegate: Kirigami.BasicListItem {
                    label: display
                    onClicked: altsView.createJob(index);
                }


                onFinished: stack.replace({
                    item: sharedComponent,
                    properties: { text: output.url },
                    replace: true
                })
            }

            Kirigami.Separator {
                Layout.fillWidth: true
            }

            Kirigami.BasicListItem {
                label: i18n("Back")
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
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            Kirigami.Label {
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

    initialItem: Item {
        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            DirectoryView {
                id: view
                header: Image {
                    fillMode: Image.PreserveAspectCrop
                    width: view.width
                    height: Kirigami.Units.gridUnit * 10
                    source: "https://images.unsplash.com/photo-1481933236927-d92e97e3194c?ixlib=rb-0.3.5&q=80&fm=jpg&crop=entropy&cs=tinysrgb&s=25a62a567881e623a7a9cd5819ed0910"

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

                Layout.fillWidth: true
                Layout.fillHeight: true
                mimeFilter: [stack.mimeFilter]
            }
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
