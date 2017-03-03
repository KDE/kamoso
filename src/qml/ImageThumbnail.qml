import org.kde.kamoso 3.0
import org.kde.kquickcontrolsaddons 2.0

QPixmapItem
{
    id: item
    property alias path: fetcher.url
    property alias mime: fetcher.mimetype

    objectName: path
    pixmap: fetcher.preview
    fillMode: QPixmapItem.PreserveAspectCrop

    readonly property int biggerSide: Math.max(width, height) * 2
    PreviewFetcher {
        id: fetcher
        width: item.biggerSide
        height: item.biggerSide
    }
}
