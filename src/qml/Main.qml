import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
//import jbQuick.Charts 1.0




Rectangle {
	id: root
	width: 800; height:600

	color: "#00FFFFFF"
//	border.color: "#F00"
//	border.width: 2
	Rectangle {
		height: 50; width: root.width
		x:0
		y:root.height-height
		color: "#AED6F1"

		Text{
			text: Number(0)
			x: 0
			color: "red"
		}
		Text{
			text: Number(10)
			x: root.width/8 * 1
			color: "red"
		}
		Text{
			text: Number(20)
			x: root.width/8 * 2
			color: "red"
		}
		Text{
			text: Number(30)
			x: root.width/8 * 3
			color: "red"
		}
		Text{
			text: Number(40)
			x: root.width/8 * 4
			color: "red"
		}
		Text{
			text: Number(50)
			x: root.width/8 * 5
			color: "red"
		}
		Text{
			text: Number(60)
			x: root.width/8 * 6
			color: "red"
		}
		Text{
			text: Number(70)
			x: root.width/8 * 7
			color: "red"
		}
		Text{
			text: Number(80)
			x: root.width
			color: "red"
		}


		//rect
		Rectangle {
		id: slideBar
		objectName: "slideBar"
		height: 50; width: 20
		property int currentX : 0
		x: currentX / 800 * root.width
//		y: root.height - height

		radius: 5
		color: "#990000FF"
			MouseArea{
				anchors.fill:parent
			
				//property bool onPress : false
				onClicked: console.log("button clicked")
			//onEntered: console.log("mouse entered")
				property bool onPress:false
				property int startX
				onPressed: {
					console.log("button pressed",  slideBar.mouseX )
					startX = mouseX
					
					onPress = true
				}
				onPositionChanged:{
					if (onPress) 
						if (slideBar.currentX + mouseX - startX<=0)
							slideBar.currentX = 0
						else
						if (slideBar.currentX + mouseX - startX>=800-slideBar.width)
							slideBar.currentX = 800 - slideBar.width
						else
							slideBar.currentX +=mouseX-startX
						
		//				startX = mouseX
//						}
				}
				onReleased:
				{
					onPress = false;
					console.log("button relased", mouseX)
				} 
			}

		}
	}
}
