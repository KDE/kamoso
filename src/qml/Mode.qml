import QtQml 2.1

QtObject {
    property var mimes: null
    property bool checkable: false
    property string icon: ""
    property string text: ""
    property string modeInfo: ""
    property string nameFilter: ""
    property var captureMode

    signal trigger(bool checked)

    default property var things: []
}
