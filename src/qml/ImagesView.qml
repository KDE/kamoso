import QtQuick 1.1
import org.kde.kamoso 3.0
import org.kde.qtextracomponents 0.1

ListView
{
	property alias mimeFilter: model.mimeFilter
	orientation: ListView.Horizontal
	
	model: DirModel {
			id: model 
			url: settings.saveUrl
	}
	
	delegate: Image { width: 40; height: 40; source: path; asynchronous: true }
}