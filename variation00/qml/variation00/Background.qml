import QtQuick 1.0

Rectangle {
  id: myBackground

  // value >0 needed (although not used because of fullscreen)
  //width: 75; height: 75
  //color: "white"
  opacity: 1.0
  anchors.fill: parent

  function randomImage() {
    return "../../img/background/"+Math.floor((Math.random()*13)+1)+".jpg";
  }

  Image {
    id: backImg
    anchors.fill: parent
    source: randomImage()
  }

  Timer {
    id: bckTimer
    interval: 5000; running: true; repeat: true
    onTriggered: {backImg.source = randomImage()}
    /*onTriggered: {  nextImage();
     animateA.start();
     animateB.start();*/
  }

  /*function nextImage(){
     flashingblob.state = "autoChange";
   }

   states: State{
     name: "autoChange";
     PropertyChanges { target: backImg; source:randomImage()}
   }

   transitions: Transition {
     NumberAnimation { properties: "source"; duration: 500; easing.type: Easing.InOutQuad  }
   }*/

  /*MouseArea {
      anchors.fill: parent
      onClicked: {
        animateColor.start()
        animateOpacity.start()
      }
    }

    PropertyAnimation {
      id: animateColor
      target: flashingblob
      properties: "color"; to: "green"; duration: 2000
    }

    NumberAnimation {
      id: animateOpacity
      target: flashingblob
      properties: "opacity"
      from: 0.99; to: 1.0
      loops: Animation.Infinite
      easing {
        type: Easing.OutBack
        overshoot: 500
      }
    }
  }*/
}

// vim: set ft=javascript: