import QtQuick 2.0

ListModel
{
    id: model
    //it would be awesome to put the trigger
    //inside the element, but didn't really see how to
    function trigger(i, checked) {
        switch(i) {
            case 0:
                whites.showAll()
                webcam.takePhoto()
                break;
            case 1:
                setBursting(checked);
                break;
            case 2:
                if (checked) {
                    webcam.startRecording()
                    break;
                }
                webcam.stopRecording()
                break;
        }
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
