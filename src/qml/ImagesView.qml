import QtQuick 2.0
import org.kde.kamoso 3.0
import org.kde.plasma.core 2.0
import org.kde.plasma.components 2.0
import org.kde.qtextracomponents 2.0

ListView
{
    property alias mimeFilter: model.mimeFilter
    orientation: ListView.Horizontal
    
    model: DirModel {
        id: model
        url: settings.saveUrl
    }
    
    delegate: ListItem {
        id: delegateItem
        width: height
        height: ListView.view.height

        enabled: true
        onClicked: Qt.openUrlExternally(path)
        IconItem {
            anchors {
                centerIn: parent
                margins: 10
            }
            clip: true
            objectName: path
            source: fetcher.preview
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
