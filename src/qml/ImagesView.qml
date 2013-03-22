import QtQuick 1.1
import org.kde.kamoso 3.0
import org.kde.plasma.core 0.1
import org.kde.qtextracomponents 0.1

ListView
{
    property alias mimeFilter: model.mimeFilter
    orientation: ListView.Horizontal
    
    model: DirModel {
        id: model
        url: settings.saveUrl
    }
    
    delegate: IconItem {
        width: fetcher.width
        height: fetcher.height
        
        PreviewFetcher {
            id: fetcher
            url: path
            mimetype: mime
            width: 40
            height: 40
        }
        objectName: path
        source: fetcher.preview
    }
}
