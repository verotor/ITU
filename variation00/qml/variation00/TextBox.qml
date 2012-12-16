// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: textbox
    width: 240
    height: 50
    radius: 5
    x: -240
    opacity: 0.7
    color: "red"

    Rectangle {
        width: 200
        height: 30
        color: "white"
        border.width: 1
        border.color: "black"
        radius: 10

        anchors { verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter }

        TextInput {
            id: textInput
            anchors { left: parent.left; leftMargin: 8; right: parent.right; rightMargin: 8; verticalCenter: parent.verticalCenter }
            focus: true
            selectByMouse: true
            validator: RegExpValidator { regExp: /\w{5,10}/ }
            Keys.onPressed: {
                if (textInput.acceptableInput) {
                    textbox.color = "green"
                } else {
                    textbox.color = "red"
                }
            }
        }
    }

    property string myContent: textInput.text
}
