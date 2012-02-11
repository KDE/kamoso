import QtQuick 1.1
import QtGStreamer 0.10
import org.kde.plasma.components 0.1

Rectangle
{
	color: "black"
	width: 500
	height: 600
	
	ImagesView {
		id: imagesView
		anchors {
			margins: 20
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}
		height: 100
	}
	
	Rectangle {
		color: "yellow"
		width: 100
		height: 40
		
		anchors.margins: 20
		anchors.left: parent.left
		anchors.bottom: imagesView.top
	}
	
	Rectangle {
		id: controls
		color: "yellow"
		width: 100
		height: 40
		
		anchors.margins: 20
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.bottom: imagesView.top
	}
	
	Row {
		width: 100
		height: 40
		
		anchors.margins: 20
		anchors.right: parent.right
		anchors.bottom: imagesView.top
		
		Button { width: 30; text: "a" }
		Button { width: 30; text: "a" }
	}
	
	VideoItem {
		anchors {
			margins: 20
			right: parent.right
			left: parent.left
			top: parent.top
			bottom: controls.top
		}
	}
}