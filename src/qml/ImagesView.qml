import QtQuick 2.0
import QtQuick.Controls 1.3
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

        delegate: MouseArea {
            id: delegateItem
            width: height
            height: scrollView.delegateWidth

            onClicked: Qt.openUrlExternally(path)
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
