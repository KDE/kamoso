import QtQml 2.2
import QtQuick 2.0
import QtQuick.Controls 1.2
import org.kde.kamoso 3.0
import org.kde.kquickcontrols 2.0
import org.kde.kquickcontrolsaddons 2.0

ScrollView
{
    id: scrollView
    property alias mimeFilter: model.mimeFilter
    signal itemClicked(string path)

    property real delegateWidth: 50
    readonly property int columnCount: Math.floor(scrollView.viewport.width/delegateWidth)

    GridView
    {
        cellWidth: scrollView.delegateWidth + (scrollView.viewport.width - columnCount*scrollView.delegateWidth)/columnCount
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
            acceptedButtons: Qt.AllButtons

            onClicked: {
                scrollView.itemClicked(path)
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
