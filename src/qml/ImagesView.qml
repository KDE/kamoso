import QtQuick 2.0
import org.kde.kamoso 3.0
import org.kde.kquickcontrols 2.0
import org.kde.kquickcontrolsaddons 2.0

ListView
{
    property alias mimeFilter: model.mimeFilter
    orientation: ListView.Horizontal
    
    model: DirModel {
        id: model
        url: settings.saveUrl
    }
    
    delegate: MouseArea {
        id: delegateItem
        width: height
        height: ListView.view.height

        onClicked: Qt.openUrlExternally(path)
        QPixmapItem {
            anchors {
                centerIn: parent
                margins: 10
            }
            clip: true
            objectName: path
            pixmap: fetcher.preview
            width: parent.width
            height: parent.height

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
