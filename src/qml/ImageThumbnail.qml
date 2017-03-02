import org.kde.kamoso 3.0
import org.kde.kquickcontrolsaddons 2.0

QPixmapItem
{
    property alias fetchWidth: fetcher.width
    property alias fetchHeight: fetcher.height
    property alias path: fetcher.url
    property alias mime: fetcher.mimetype

    objectName: path
    pixmap: fetcher.preview

    PreviewFetcher {
        id: fetcher
    }
}
