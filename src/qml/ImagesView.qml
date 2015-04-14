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
            text: i18n("Open...")
            onTriggered: Qt.openUrlExternally(menu.title)
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
                onTriggered: {
                    var job = altsModel.createJob(index)

                    if (job.isReady)
                        startShareJob(job)
                    else {
                        stack.push({
                            item: shareWizardComponent,
                            properties: { job: job }
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

    function startShareJob(job) {
        stack.push({
            item: busyComponent
        });

        job.start();
        job.output.connect(function(output) {
            job.outputUrl = output.url
        });
        job.result.connect(function(job) {
            console.log("lalalala", job.outputUrl)
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
            property alias job: wiz.job
            PurposeWizard {
                id: wiz

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            RowLayout {
                Button {
                    text: i18n("Run")
                    enabled: wiz.job && wiz.job.isReady
                    onClicked: {
                        stack.pop();
                        startShareJob(wiz.job);
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
        BusyIndicator {
            running: true
        }
    }
}
