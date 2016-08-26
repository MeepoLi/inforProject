import QtQuick 2.0
import QtCharts 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2




Rectangle {
	id: root
	width: 0; height: 0

	color: "#00FFFFFF"
//	border.color: "#F00"
//	border.width: 2
	Rectangle {
		id: timeBar
		objectName: "timeBar"
		height: 50; width: root.width
		x:0
		y:root.height-height-chart.height
		property int n:100
		property int offset: 20
		color: "#AED6F1"

		Text{
			text: Number(0)
			x: 0
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.1))
			x: (root.width - timeBar.offset)*0.1
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.2))
			x: (root.width -  timeBar.offset)*0.2
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.3))
			x: (root.width- timeBar.offset)*0.3
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.4))
			x: (root.width -  timeBar.offset)*0.4
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.5))
			x: (root.width -  timeBar.offset)*0.5
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.6))
			x: (root.width -  timeBar.offset)*0.6
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.7))
			x: (root.width -  timeBar.offset)*0.7
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.8))
			x: (root.width - timeBar.offset) * 0.8
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n*0.9))
			x: (root.width - timeBar.offset) * 0.9
			color: "red"
		}
		Text{
			text: Number(Math.round(timeBar.n))
			x: (root.width - timeBar.offset)
			color: "red"
		}


		//rect
		Rectangle {
		id: slideBar
		objectName: "slideBar"
		height: 50; width: 20
		property int currentX : 0
		property int timeStamp : currentX / 800 * timeBar.n
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


	ChartView{
		id: chart
		title: "chart"
		objectName: "chart"
		width: root.width
		height: 0
		x:0
		y:root.height - height
	}


}
