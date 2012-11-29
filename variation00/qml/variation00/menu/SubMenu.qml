import QtQuick 1.0
//import Qt.labs.particles 1.0

MyMenu {
  id: subMenu
  visible: true
  property bool closed: true
  property int midX: 0
  property int midY: 0

  property int quadrant: 1 //?????????????????
  property int diameter: 0
  property real angle: 0

  function getCoord(axis, closing) {
    if (closing) {
      return (axis == "x") ? subMenu.midX - subMenu.width/2 :
                             subMenu.midY - subMenu.height/2
    }
    else {
      var xx, yy
      yy = Math.sin(subMenu.angle) * subMenu.diameter
      xx = Math.sqrt(Math.pow(subMenu.diameter, 2) - Math.pow(yy, 2))

      if (axis == "x")
        return subMenu.midX + xx
      else
        return subMenu.midY - yy
    }
  }

  onClosedChanged: {
    if (behavX.running || behavY.running) return

    if (closed) {
      behavX.to = getCoord("x", true)
      behavY.to = getCoord("y", true)
      behavX.running = true
      behavY.running = true
      //FIXME zkusit zaroven opacity
    }
    else {
      subMenu.opacity = 1
      behavX.to = getCoord("x", false)
      behavY.to = getCoord("y", false)
      behavX.running = true
      behavY.running = true
    }
  }

  /* FIXME submenu == menu, ktere pri zmene na visible vyjede a pri zmene na
     invisible zajede a pri stlaceni se jeste malinko promackne (zmensi se) */
  SpringAnimation {
    id: behavX
    target: subMenu
    properties: "x"
    spring: 10
    damping: 0.4
    onRunningChanged: {
      if (!running && closed) subMenu.opacity = 0
    }
  }
  SpringAnimation {
    id: behavY
    target: subMenu
    properties: "y"
    spring: 10
    damping: 0.4
    //onRunningChanged: {
    //  console.log("to " + to.toString() + " HEEEEEEEEEEEEEEEEEEEEEEEEEJ")
    //}
  }
  //Behavior on x {
  //  id: behavX
  //  //enabled: true
  //  SpringAnimation {
  //    spring: 4
  //    damping: 0.1
  //  }
  //}

  //Behavior on y {
  //  id: behavY
  //  //enabled: true
  //  SpringAnimation {
  //    spring: 4
  //    damping: 0.1
  //    onRunningChanged: {
  //      if (!running && subMenu.closed) subMenu.visible = false
  //    }
  //  }
  //  //NumberAnimation {
  //  //  id: fadeOut
  //  //  target: topLayer
  //  //  properties: "opacity"
  //  //  to: 0
  //  //  duration: myBg.fadeDuration
  //  //  onRunningChanged: {
  //  //    /* just finished animation */
  //  //    if (!running && !paused) {
  //  //      topLayer.source = bottomLayer.source
  //  //      topLayer.opacity = 1
  //  //    }
  //  //  }
  //  //}
  //}
}

/*
Item {
  id: submenuItem
  property bool dying: false
  property bool spawned: false
  property string icon: "dummy"

  Behavior on x {
    enabled: spawned;
    SpringAnimation { spring: 4; damping: 0.1 }
  }
  Behavior on y {
    SpringAnimation { spring: 4; damping: 0.1 }
  }

  function imgSrc() {
    return "../../../img/menu/" + icon + "0.png"
  }

  Image {
    id: img
    source: imgSrc()
    anchors.fill: parent
    opacity: 0

    Behavior on opacity { NumberAnimation { duration: 200 } }
  }

  Particles {
    id: particles
    width: 3; height: 3
    anchors.centerIn: parent

    emissionRate: 0
    lifeSpan: 700; lifeSpanDeviation: 600
    angle: 0; angleDeviation: 360;
    velocity: 100; velocityDeviation: 30
    source: imgSrc()
  }

  states: [
    State {
      name: "AliveState"
      when: spawned == true && dying == false
      PropertyChanges { target: img; opacity: 1 }
    },
    State {
      name: "DeathState"
      when: dying == true
      StateChangeScript { script: particles.burst(50); }
      PropertyChanges { target: img; opacity: 0 }
      //StateChangeScript { script: submenuItem.destroy(1000); }
    }
  ]
}
*/