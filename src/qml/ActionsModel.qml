import QtQuick 1.1

ListModel
{
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
        text: "Shoot"
        mimes: "image/png"
        checkable: false
    }
    ListElement {
        icon: "code-blocks"
        text: "Burst"
        mimes: "image/png"
        checkable: true
    }
    ListElement {
        icon: "media-record"
        text: "Action!"
        mimes: "video/x-matroska"
        checkable: true
    }
}
