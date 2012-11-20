import QtQuick 1.0
import QtWebKit 1.0

Rectangle {
  id: background

  // value >0 needed (although not used because of fullscreen)
  width: 75; height: 75
  //color: "white"
  opacity: 1.0

  function randomImage(){
    return "Images/background/"+Math.floor((Math.random()*13)+1)+".jpg";
  }

  Image {
    id: background
    anchors.fill: parent
    source: randomImage()
  }

  Timer {
    id: bckTimer
    interval: 5000; running: true; repeat: true
    onTriggered: {background.source = randomImage()}
    /*onTriggered: {  nextImage();
     animateA.start();
     animateB.start();*/
   }


   WebView {
     id: translationsList

     anchors {
       topMargin:60
       bottomMargin:60
       leftMargin: 60
       rightMargin: 60
       bottom:parent.bottom
       right:parent.right
       left:parent.left
       top:parent.top
     }

     url: Qt.resolvedUrl( "html/index.html" )
   }

   /* url: Qt.resolvedUrl( "html/index.html" )
    x: 0
    y: 0
    smooth: false
    anchors {
      top: window.top
      bottom: window.bottom
      left: window.left
      right: window.right
    }*/
  }

  /*function nextImage(){
     flashingblob.state = "autoChange";
   }

   states: State{
     name: "autoChange";
     PropertyChanges { target: background; source:randomImage()}
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

// vim: set ft=javascript:
