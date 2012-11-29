import QtQuick 1.0
//import Qt.labs.particles 1.0

MyMenu {
  id: mySubMenu
  visible: false
  property int aimX: 0
  property int aimY: 0

  /* FIXME submenu == menu, ktere pri zmene na visible vyjede a pri zmene na
     invisible zajede a pri stlaceni se jeste malinko promackne (zmensi se) */
  Behavior on x {
    id: behavX
    enabled: false
    SpringAnimation { spring: 4; damping: 0.1 }
  }

  Behavior on y {
    id: behavY
    enabled: false
    SpringAnimation { spring: 4; damping: 0.1 }
  }

  onVisibleChanged: {
    if (subMenu.visible) {
      behavX.enabled = true
      behavY.enabled = true
      console.log("submenu visible")
    }
    else {
      behavX.enabled = false
      behavY.enabled = false
      console.log("submenu INvisible")
    }

    var tmpx = x; var tmpy = y
    x = aimX; y = aimY
    aimX = tmpx; aimY = tmpy
  }
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