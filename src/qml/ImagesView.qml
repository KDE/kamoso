import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.kamoso 3.0
import org.kde.kamoso.share 3.0
import org.kde.kquickcontrols 2.0
import org.kde.kquickcontrolsaddons 2.0

ScrollView
{
    id: scrollView
    property alias mimeFilter: model.mimeFilter
    property real delegateWidth: 50

    GridView
    {
        cellWidth: scrollView.delegateWidth
        cellHeight: cellWidth
        anchors.fill: parent

        model: DirModel {
            id: model
            url: settings.saveUrl
        }

        Menu {
            id: menu
            MenuItem {
                text: i18n("Open...")
                onTriggered: Qt.openUrlExternally(menu.title)
            }
            MenuSeparator {}
            MenuItem {
                text: "xxxx"+inst.count
            }

            Instantiator {
                id: inst
                model: ShareAlternativesModel {
                    mimeTypes: model.mimeFilter
                }
                MenuItem {
                    text: display
                }

                onObjectAdded: menu.insertItem(menu.items.count, object)
                onObjectRemoved: menu.removeItem(object)
            }
        }

        delegate: MouseArea {
            id: delegateItem
            width: height
            height: scrollView.delegateWidth

            onClicked: {
                menu.visible = false
                menu.title = path
                menu.popup()
            }

            QPixmapItem {
                anchors {
                    fill: parent
                    margins: 1
                }
                objectName: path
                pixmap: fetcher.preview

                PreviewFetcher {
                    id: fetcher
                    url: path
                    mimetype: mime
                    width: delegateItem.width
                    height: delegateItem.height
                }
            }
        }
    }
}
