import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.kamoso 3.0
import org.kde.kamoso.share 3.0

StackView {
    id: stack
    property alias mimeFilter: view.mimeFilter

    Menu {
        id: menu
        MenuItem {
            text: i18n("Open...")
            onTriggered: Qt.openUrlExternally(menu.title)
        }
        MenuSeparator {}

        Instantiator {
            id: inst
            model: ShareAlternativesModel {
                id: altsModel
                mimeTypes: view.mimeFilter
            }
            MenuItem {
                text: display
                onTriggered: {
                    var job = altsModel.createJob(index)
                    job.data = { "urls": [ menu.title ] }
                    if (job.isReady)
                        job.start()
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

    Component {
        id: shareWizardComponent
        ShareWizard {
            onAccepted: {
                stack.pop();
                job.start();
            }
        }
    }
}
