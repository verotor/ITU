// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Item {
  id: myMenu
  property string icon: "dummy"
  //property alias operation: buttonText.text
  signal mouseClick

  function imgSrc(mode) {
    return "../../../img/menu/" + myMenu.icon + mode.toString() + ".png"
  }

  Rectangle {
    id: shade
    anchors.fill: parent
    radius: image.width
    color: "black"
    opacity: 0
  }

  //FIXME animace "pomale pulsovani"
  Image {
    id: image
    anchors {
      fill: parent;
      //fillMode: "PreserveAspectFit"
      topMargin: 16; bottomMargin: 16; leftMargin: 16; rightMargin: 16
    }
    //z: 2
    smooth: true
    source: imgSrc(0)
    clip: false
  }

  MouseArea {
    id: mouseArea
    anchors.fill: parent
    hoverEnabled: true
    onClicked: mouseClick()
  }

  /* only the first matching state (according to "when") is chosen */
  states: [
    State {
      name: "pressed"
      when: mouseArea.pressed == true
      PropertyChanges { target: shade; opacity: .4 }
      PropertyChanges {
        target: image; source: imgSrc(0)
      }
    },
    State {
      name: "mouse_onEntered"
      when: mouseArea.containsMouse == true
      PropertyChanges {
        target: image; source: imgSrc(1)
      }
    }
  ]
}