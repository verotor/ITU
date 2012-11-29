import QtQuick 1.0

Rectangle {
  id: myBg
  anchors.fill: parent

  property int fadeDuration: 1000
  property int changeInterval: 4000
  property bool isPaused: false

  onIsPausedChanged: {
    bckTimer.running = (myBg.isPaused) ? false : true
    if (fadeIn.running) fadeIn.paused = (myBg.isPaused) ? true : false
    if (fadeOut.running) fadeOut.paused = (myBg.isPaused) ? true : false
    coverLayer.opacity = (myBg.isPaused) ? 0.6 : 0
  }

  function randomImage() {
    return "../../img/background/"+Math.floor((Math.random()*13)+1)+".jpg";
  }

  Timer {
    id: bckTimer
    interval: changeInterval
    running: true
    repeat: true
    onTriggered: {
      bottomLayer.source = randomImage()
      //fadeIn.start()
      fadeOut.start()
    }
  }

  /* fade in animation */
  Image {
    id: bottomLayer
    anchors.fill: parent
    //source: randomImage()

    NumberAnimation {
      id: fadeIn
      target: bottomLayer
      properties: "opacity"
      from: 0
      to: 1
      duration: myBg.fadeDuration
    }
  }

  Image {
    id: topLayer
    anchors.fill: parent
    source: randomImage()

    NumberAnimation {
      id: fadeOut
      target: topLayer
      properties: "opacity"
      to: 0
      duration: myBg.fadeDuration
      onRunningChanged: {
        /* just finished animation */
        if (!running && !paused) {
          topLayer.source = bottomLayer.source
          topLayer.opacity = 1
        }
      }
    }
  }

  /* paused yields darker look */
  Rectangle {
    id: coverLayer
    anchors.fill: parent
    color: "#000000"
    opacity: 0.6
  }
}

// vim: set ft=javascript: