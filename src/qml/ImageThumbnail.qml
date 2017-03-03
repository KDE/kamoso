import org.kde.kamoso 3.0
import org.kde.kquickcontrolsaddons 2.0

QPixmapItem
{
    property alias path: fetcher.url
    property alias mime: fetcher.mimetype

    objectName: path
    pixmap: fetcher.preview

    PreviewFetcher {
        id: fetcher
        width: parent.width
        height: parent.height
    }
}
