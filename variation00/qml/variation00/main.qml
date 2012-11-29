import QtQuick 1.1
import QtWebKit 1.0
//import Effects 1.0
import "menu/"

// qsTr("some text") used for translation

Rectangle {
  id: win_main
  width: 1; height: 1;
  color: "#CC77FF"
  focus: true  // needed for keyboard keys catching

  Background {
    id: mainBg
    isPaused: true
    onIsPausedChanged: {
      /* PAUSE & stisknete tlacitko... messages */
      pauseMsgs.visible = (mainBg.isPaused) ? true : false
      /* copyleft msg */
      copyrightMsg.visible = (mainBg.isPaused) ? true : false
    }
  }

  /* catch mouse and un-pause the game on whatever click */
  MouseArea {
    anchors.fill: parent
    onClicked: if (mainBg.isPaused) mainBg.isPaused = false
  }

  /* catch keyboard and un-pause the game on any keypress */
  Keys.onPressed: {
    if (event.key == Qt.Key_Shift ||
        event.key == Qt.Key_Control ||
        event.key == Qt.Key_Alt ||
        event.key == Qt.Key_Meta ||
        event.key == Qt.Key_Menu ||
        event.key == Qt.Key_Keypad) return
    if (mainBg.isPaused) mainBg.isPaused = false
  }

  /*
  Help { }
  */

  /*
  Flickable {
    id: wtf00
    width: parent.width
    height: parent.height
    contentWidth: web00.width
    contentHeight: web00.height
    interactive: true
    //clip: true

    WebView {
      id: web00
      url: "http://www.qtcentre.org/threads/46192-QML-webview-navigation"
      width: win_main.width
      //height: win_main.height
      //preferredWidth: flickable.width
      //preferredHeight: flickable.height
      x: 0
      y: 0
    }
  }
  */

  Column {
    id: pauseMsgs
    anchors.centerIn: parent
    spacing: 10

    Text {
      anchors.horizontalCenter: parent.horizontalCenter
      color: "red"
      font.bold: true
      font.pixelSize: win_main.height / 8
      style: Text.Outline
      styleColor: "black"
      //font.pointSize: 40.78
      text: "PAUZA"
      smooth: true
    }

    Text {
      anchors.horizontalCenter: parent.horizontalCenter
      color: "yellow"
      style: Text.Outline
      styleColor: "black"
      smooth: true
      text: "stisknete tlacitko na klavesnici nebo mysi pro pokracovani ve hre"
    }
  }

  //FIXME pridat bublinovou napovedu? (ta by vsak byla na skodu -
  //  prekazela by - na mobilnich zarizenich
  MyMenu {
    id: menu_exit;
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: win_main.height / 7; height: win_main.height / 7;
    icon: "menu_exit"
    onMouseClick: {
      console.log("exit button pressed from main window")
      //FIXME nejak rekurzivne zavrit cele menu!
      subMExit00.visible = (subMExit00.visible) ? false : true
      //Qt.quit();
    }
  }

  SubMenu {
    id: subMExit00
    anchors.bottom: menu_exit.bottom
    anchors.left: menu_exit.left
    width: menu_exit.width / 1.2
    height: menu_exit.height / 1.2

    aimX: menu_exit.x + menu_exit.width
    aimY: menu_exit.y - menu_exit.height
    icon: "menu_exit"
    onMouseClick: { console.log("exit submenu clicked\n" + subMExit00.x + " " + subMExit00.y) }
  }

  Text {
    id: copyrightMsg;
    //anchors.horizontalCenter: parent
    anchors.bottomMargin: 30
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    smooth: true
    color: "#FF6633"
    text: "(c) Radek, Lukas, Honza"
  }

  MyMenu {
    id: menu_main;
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: win_main.height / 6.2; height: win_main.height / 6.2;
    icon: "menu_main"

    //FIXME testovani "pause"
    onMouseClick: {
      mainBg.isPaused = true
      console.log("main button clicked")
    }

    //FIXME spojit pauzu s vypnutim rotace?
    RotationAnimation on rotation {
      duration: 20000
      loops: Animation.Infinite
      from: 0
      to: 360
    }

    /*
    transform: Rotation {
      id: testRotation
      origin.x: myMenu.width / 2
      origin.y: myMenu.height / 2;
      angle: 15
      Behavior on angle {
        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
      }
    }
    */
  }

  // pozadi == zasedla fotka (stretch)
  // vlevo, uprostred, vpravo dole obrazky
  //   pruhlednost zachovana
  // uprostred (horizontalne, vertikalne) <bold red big pulsing text>PAUSE</><small black text>maly</>
  // mouse hover
  //   na ikony menu_exit/menu_main -> priblizeni/???
  // mouse click
  //   na obrazek -> zmizet (menu_exit + copyright + PAUSE)
  //   na ikony menu_exit/menu_main spusti animaci a otevre prislusna submenu
}

/*
Rectangle {
  id: flashingblob

  // value >0 mandatory (although not used because of fullscreen)
  width: 75; height: 75
  color: "blue"
  opacity: 1.0

  MouseArea {
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
}
*/

/*
Rectangle{
  id:fullScreen
  width: fullScreenImage.width
  height: fullScreenImage.height
  color: "transparent"
  x:0;y:50;
  Image{
    id:fullScreenImage
    source: "pics/fullScreenDown.png"
  }
  MouseArea{
    id:fullScreenArea
    anchors.fill: fullScreenImage
    onPressed: {
      fscreen=1;
      fullScreenImage.source="pics/fullScreenUp.png"
      showhanddown(3)
      fullscreen(k)
      //playIcon.x=250
      //playIcon.y=200
    }
    onReleased: {
      fullScreenImage.source="pics/fullScreenDown.png"
      showhanddown(0)
    }
  }
}
*/

// vim: set ft=javascript: