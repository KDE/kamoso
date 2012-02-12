import QtQuick 1.1

ListModel
{
    //it would be awesome to put the trigger
    //inside the element, but didn't really see how to
    function trigger(i, checked) {
        switch(i) {
            case 0:
                whites.showAll()
                console.log("photoooooooo!")
                webcam.takePhoto()
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
    }

    ListElement {
        icon: "weather-clear"
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
        icon: "audio-input-microphone"
        text: "Action!"
        mimes: "video/x-matroska"
        checkable: true
    }
}
