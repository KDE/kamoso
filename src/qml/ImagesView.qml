import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.kamoso 3.0
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
                text: i18n("Open %1", menu.title)
                onTriggered: Qt.openUrlExternally(menu.title)
            }
            MenuSeparator {}
            MenuItem { text: "sharing" }
            MenuItem { text: "to" }
            MenuItem { text: "be" }
            MenuItem { text: "imlpemented" }
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
