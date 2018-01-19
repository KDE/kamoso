import org.kde.kirigami 2.0

Action {
    id: modeAction
    property string mimes: null
    property string name: ""
    property string modeInfo: ""
    property string nameFilter: ""

    readonly property Action adoptAction: Action {
        text: modeAction.text
        iconName: modeAction.iconName
        tooltip: modeAction.tooltip
        onTriggered: {
            visor.state = modeAction.iconName
        }
    }

    default property var things: []
}
