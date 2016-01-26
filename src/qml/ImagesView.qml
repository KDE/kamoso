import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import org.kde.kamoso 3.0
import org.kde.purpose 1.0

StackView {
    id: stack
    property string mimeFilter
    property alias nameFilter: view.nameFilter
    clip: true

    Menu {
        id: menu
        Instantiator {
            id: inst
            model: PurposeAlternativesModel {
                id: altsModel
                pluginType: "Export"
                inputData: {
                    "urls": view.selection,
                    "mimeType": stack.mimeFilter
                }
            }
            MenuItem {
                text: display
                iconName: model.iconName
                onTriggered: {
                    var config = altsModel.configureJob(index)

                    if (config.isReady)
                        startShareJob(config)
                    else {
                        stack.push({
                            item: shareWizardComponent,
                            properties: { configuration: config }
                        })
                    }
                    view.selection = []
                }
            }

            onObjectAdded: menu.insertItem(menu.items.count, object)
            onObjectRemoved: menu.removeItem(object)
        }
    }

    initialItem: Rectangle {
        ColumnLayout {
            anchors.fill: parent
            ToolBar {
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.fillWidth: true
                        text: view.selection.length==0 ? i18n("Gallery") : i18n("%1 selected", view.selection.length)
                    }
                    ToolButton {
                        iconName: "user-trash"
                        tooltip: i18n("Move to trash...")
                        visible: view.selection.length>0
                        onClicked: {
                            trashDialog.visible = true
                        }

                        Dialog {
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
                    ToolButton {
                        iconName: "document-share"
                        menu: menu
                        tooltip: i18n("Share...")
                        visible: view.selection.length>0
                    }
                    ToolButton {
                        iconName: "folder-open"
                        tooltip: i18n("Open Folder")
                        onClicked: Qt.openUrlExternally(config.saveUrl)
                    }
                }
            }
            DirectoryView {
                id: view

                Layout.fillWidth: true
                Layout.fillHeight: true
                mimeFilter: [stack.mimeFilter]
            }
        }
    }

    function startShareJob(config) {
        var job = config.createJob();
        stack.push({
            item: busyComponent,
            properties: { job: job }
        });

        job.start();
        job.result.connect(function(job) {
            if (job.output.url=="")
                return;
            stack.replace({
                item: sharedComponent,
                properties: { text: job.output.url },
                replace: true
            })
        });
    }

    Component {
        id: sharedComponent
        ColumnLayout {
            property alias text: field.text
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
            Label {
                Layout.fillHeight: true
                text: i18n("The URL was just shared")
            }
            Button {
                text: i18n("Back")
                onClicked: {
                    stack.pop()
                }
            }
        }
    }

    Component {
        id: shareWizardComponent
        ColumnLayout {
            property alias configuration: wiz.configuration
            PurposeWizard {
                id: wiz

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            RowLayout {
                Button {
                    text: i18n("Run")
                    enabled: wiz.configuration && wiz.configuration.isReady
                    onClicked: {
                        stack.pop();
                        startShareJob(wiz.configuration);
                    }
                }
                Button {
                    text: i18n("Back")
                    onClicked: {
                        stack.pop();
                        wiz.cancel()
                    }
                }
            }
        }
    }

    Component {
        id: busyComponent
        ColumnLayout {
            property QtObject job

            BusyIndicator {
                Layout.fillWidth: true
                Layout.fillHeight: true
                running: true
            }

            Button {
                anchors.right: parent.right
                text: i18n("Cancel")
                enabled: parent.job && (parent.job.capabilities & KJob.Killable)

                onClicked: {
                    if (parent.job.kill()) {
                        stack.pop()
                    }
                }
            }
        }
    }
}
