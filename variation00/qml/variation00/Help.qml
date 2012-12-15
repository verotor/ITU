import QtQuick 1.0
import QtWebKit 1.0

Rectangle {
  id: myHelp
  width: 100
  height: 62
  anchors.fill: parent

  Flickable {
    id: wtf00
    width: parent.width
    height: parent.height
    contentWidth: web00.width
    contentHeight: web00.height
    interactive: true
    anchors {
      left:parent.left
    }

    WebView {
      id: web00
      url: Qt.resolvedUrl( "html/index.html" )
      width: flashingblob.width
      //height: win_main.height
      //preferredWidth: flickable.width
      //preferredHeight: flickable.height
      x: 0
      y: 0
    }
  }
}