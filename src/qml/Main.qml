import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
//import jbQuick.Charts 1.0


//Text {
//    id: output
//    text: qsTr("Hello World")
//    anchors.centerIn: parent
//}



Rectangle {
	id: root
	width: 800; height:600

	color: "#00FFFFFF"
//	border.color: "#F00"
//	border.width: 2
	Text {
		id: text
		//text: "hello"
		y:30
		anchors.horizontalCenter: page.horizontalCenter
		font.pointSize: 24; font.bold: true
	}
	Rectangle {
		height: 50; width: root.width
		x:0
		y:root.height-height
		color: "#FFFFFF"
	}
	Rectangle {
		id: ball
		objectName: "ball"
		height:  50; width: 10
		x: 0
		y: root.height - height
		color: "#990000FF"
	}
	SequentialAnimation {
		running: true; loops: Animation.Infinite
	
		//NumberAnimation{ target: ball; property: "x"; to: root.width - ball.width  ; duration:1000; easing.type: Easing.OutBounce}
		//PauseAnimation{ duration:1000 }
		//NumberAnimation{ target: ball; property: "x"; to: 0; duration: 1000; easing.type: Easing.OutBounce }
		//PauseAnimation{ duration: 1000}
	
	}
}
