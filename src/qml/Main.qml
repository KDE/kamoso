import QtQuick 1.1
import QtGStreamer 0.10
import org.kde.plasma.components 0.1

Rectangle
{
	color: "white"
	width: 500
	height: 600

	ImagesView {
		id: imagesView
		mimeFilter: modes.checkedButton.stuff.mimes
		anchors {
			margins: 20
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}
		height: 30
	}

	ButtonRow {
		id: modes
		width: 100
		spacing: 10

		anchors.margins: 20
		anchors.left: parent.left
        anchors.verticalCenter: controls.verticalCenter

		Repeater {
			model: ActionsModel { id: actions }
			delegate: Button {
				property QtObject stuff: model
				
				iconSource: icon
			}
		}
	}

	Button {
		id: controls
		width: 100
		height: 40
		iconSource: modes.checkedButton.stuff.icon
		checkable: modes.checkedButton.stuff.checkable

		anchors.margins: 10
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.bottom: imagesView.top
		
		onClicked:{
            actions.trigger(modes.checkedButton.stuff.index, checked)
        }
	}

	Row {
		width: 100

		anchors.margins: 20
		anchors.right: parent.right
		anchors.verticalCenter: controls.verticalCenter
		spacing: 10

		Button { width: 30; text: "a" }
		Button { width: 30; text: "a" }
	}

	VideoItem {
        id: video

        surface: videoSurface1
        anchors {
            right: parent.right
            left: parent.left
            top: parent.top
            bottom: controls.top
        }
    }
}
