import QtQuick 1.0
import QtWebKit 1.0

/*
Rectangle {
  id: myHelp
  width: 100
  height: 62
  anchors.fill: parent
}
*/

WebView {
  id: myHelp

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
  /*
    x: 0
    y: 0
    smooth: false
    anchors {
      top: window.top
      bottom: window.bottom
      left: window.left
      right: window.right
    }
    */
}