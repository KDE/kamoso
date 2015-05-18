import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import org.kde.kamoso 3.0
import org.kde.purpose 1.0

StackView {
    id: stack
    property alias mimeFilter: view.mimeFilter
    clip: true

    Menu {
        id: menu
        MenuItem {
            text: menu.title
            enabled: false
        }
        MenuSeparator {}

        MenuItem {
            text: i18n("Open...")
            onTriggered: Qt.openUrlExternally(menu.title)
        }
        MenuItem {
            text: i18n("Open Directory...")
            onTriggered: Qt.openUrlExternally(config.saveUrl)
        }

        MenuSeparator {}

        Instantiator {
            id: inst
            model: PurposeAlternativesModel {
                id: altsModel
                pluginType: "Export"
                inputData: {
                    "urls": [ menu.title ],
                    "mimeType": view.mimeFilter
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
                }
            }

            onObjectAdded: menu.insertItem(menu.items.count, object)
            onObjectRemoved: menu.removeItem(object)
        }
    }

    initialItem: DirectoryView {
        id: view
        onItemClicked: {
            menu.visible = false
            menu.title = path
            menu.popup()
        }
    }

    function startShareJob(config) {
        var job = config.createJob();
        stack.push({
            item: busyComponent,
            properties: { job: job }
        });

        job.start();
        job.output.connect(function(output) {
            job.outputUrl = output.url
        });
        job.result.connect(function(job) {
            if (job.outputUrl=="")
                return;
            stack.replace({
                item: sharedComponent,
                properties: { text: job.outputUrl },
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
