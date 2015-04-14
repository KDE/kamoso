import QtQuick 2.0

ListModel
{
    id: model
    //it would be awesome to put the trigger
    //inside the element, but didn't really see how to
    function trigger(i, checked) {
        var ret = "";
        switch(i) {
            case 0:
                whites.showAll()
                console.log("photoooooooo!")
                ret = webcam.takePhoto()
                break;
            case 1:
                console.log("burst!!! "+checked)
                break;
            case 2:
                if (checked) {
                    webcam.startRecording()
                    break;
                }
                webcam.stopRecording()
                break;
        }
        return ret
    }

    ListElement {
        icon: "camera-web"
        text: ""
        mimes: "image/jpeg"
        checkable: false
    }
    ListElement {
        icon: "code-function"
        text: ""
        mimes: "image/jpeg"
        checkable: true
    }
    ListElement {
        icon: "media-record"
        text: ""
        mimes: "video/x-matroska"
        checkable: true
    }

    Component.onCompleted: {
        model.get(0).text = i18n("Shoot");
        model.get(1).text = i18n("Burst");
        model.get(2).text = i18n("Action!");
    }
}
