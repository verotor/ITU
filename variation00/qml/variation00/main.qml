import QtQuick 1.0
import QtWebKit 1.0
//import Effects 1.0
import "menu/"
import StarMenu 1.0

// qsTr("some text") used for translation

// mouse click
//   na obrazek -> zmizet (menu_exit + copyright + PAUSE)
//   na ikony menu_exit/menu_main spusti animaci a otevre prislusna submenu

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

  //Help { }

/*
  Flickable {
    id: wtf00
    interactive: true
    visible: true

    //contentWidth: web00.width
    contentHeight: web00.height
    anchors {
      fill: parent
      leftMargin: 70
      rightMargin: 70
      bottomMargin: 70
      topMargin: 70
    }

    WebView {
      id: web00
      url: Qt.resolvedUrl( "http://www.bungie.net/projects/aerospace/crimson/content.aspx?link=crimson_screens" )
      //url: Qt.resolvedUrl( "html/index.html" )
      //anchors.fill: parent
      //width: win_main.width - 50
      //height: win_main.height - 50

      //preferredWidth: flickable.width
      //preferredHeight: flickable.height
      preferredWidth: wtf00.width
      preferredHeight: wtf00.height
      //x: 0
      //y: 0
    }
  }
*/

  SubMenu {
    id: menuExitSub0
    width: menu_exit.width
    height: menu_exit.height
    midX: menu_exit.x + menu_exit.width/2
    midY: menu_exit.y + menu_exit.height/2
    x: midX
    y: midY
    opacity: 0

    quadrant: 1
    diameter: menu_exit.width * 1.09
    angle: Math.PI/3
    icon: "menu_exit"
    onMouseClick: {
      console.log("exit submenu clicked\n")
      Qt.quit();
    }
  }

  MyMenu {
    id: menu_exit
    anchors {
      bottom: parent.bottom
      left: parent.left
      leftMargin: 16
      bottomMargin: 16
    }
    width: win_main.height / 9; height: win_main.height / 9;
    icon: "menu_exit"
    onMouseClick: {
      //FIXME nejak rekurzivne zavrit cele menu!
      if (menuExitSub0.closed)
        menuExitSub0.closed = false
      else
        menuExitSub0.closed = true
    }
    //Component.onCompleted: {
    //  menuExitSub0.midX = menu_exit.x + menu_exit.width/2
    //  menuExitSub0.midY: menu_exit.y + menu_exit.height/2
    //}
  }

  MyMenu {
    id: menu_main;
    anchors {
      bottom: parent.bottom
      right: parent.right
      rightMargin: 16
      bottomMargin: 16
    }
    width: win_main.height / 8; height: win_main.height / 8;
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
  }

  StarMenu {
    id: starMenu
    objectName: "Menu1"
    anchors.fill: menu_main
    smooth: true
    radiusH: 100
    radiusV: 100
    starCenterVisible: true
    visualParent: win_main  // where to click to close starmenu
    color: Qt.rgba(0, 0, 0, 0.5)

    center.x: menu_main.width>>2
    center.y: menu_main.height>>1

    onStarMenuOpening: {
      columnItem.ppx=center.x; columnItem.ppy=center.y;
      textRect.z = starMenu.z;
      textRect.text = "WTFFFFFFFFFFFFFFFFFFF?"
    }

    StarItem {
      width: 64; height: 64
      onClicked: console.log("HOVER ITEM STARITEM TEEXT CLICKED");

      Image {
        anchors.fill: parent; smooth: true
        fillMode: Image.Stretch
        source: "../../img/menu/menu_exit0.png"
      }
    }
  }
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
