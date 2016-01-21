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

    readonly property real delegateWidth: 70
    readonly property int columnCount: Math.floor(scrollView.viewport.width/delegateWidth)
    property var selection: []

    GridView
    {
        cellWidth: scrollView.delegateWidth + (scrollView.viewport.width - columnCount*scrollView.delegateWidth)/columnCount
        cellHeight: cellWidth
        anchors.fill: parent

        model: DirModel {
            id: model
            url: config.saveUrl
        }
        onCountChanged: {
            scrollView.selection = []
        }

        delegate: MouseArea {
            id: delegateItem
            width: height
            height: scrollView.delegateWidth
            acceptedButtons: Qt.AllButtons

            onClicked: {
                var url = path.toString();
                var idx = scrollView.selection.indexOf(url);
                if (idx < 0) {
                    scrollView.selection.push(url)
                } else {
                    scrollView.selection.splice(idx, 1)
                }
                scrollView.selectionChanged(scrollView.selection);
            }

            Rectangle {
                anchors {
                    margins: -1
                    fill: parent
                }
                visible: scrollView.selection.indexOf(path.toString())>=0

                SystemPalette {
                    id: pal
                }
                color: pal.highlight
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
