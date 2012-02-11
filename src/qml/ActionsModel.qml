import QtQuick 1.1

ListModel
{
    //it would be awesome to put the trigger
    //inside the element, but didn't really see how to
    function trigger(i) {
        switch(i) {
            case 0:
                console.log("photoooooooo!")
                break;
            case 1:
                console.log("burst!!!")
                break;
            case 2:
                console.log("video!")
                break;
        }
    }

    ListElement {
        icon: "weather-clear"
        text: "Shoot"
        mimes: "image/png"
    }
    ListElement {
        icon: "code-blocks"
        text: "Burst"
        mimes: "image/png"
    }
    ListElement {
        icon: "audio-input-microphone"
        text: "Action!"
        mimes: "video/x-matroska"
    }
}
