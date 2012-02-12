import QtQuick 1.1

ListModel
{
    //it would be awesome to put the trigger
    //inside the element, but didn't really see how to
    function trigger(i, checked) {
        var ret = "http://images3.wikia.nocookie.net/__cb20110211005027/thewargame/images/3/31/Dancing_Banana.gif";
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
