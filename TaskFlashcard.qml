import QtQuick

Rectangle {
    id: root

    property int positionX
    property int positionY

    x: positionX * Window.width/1072
    y: positionY * Window.height/603

    property real scale: 1.0
    property string question: ""
    property string answer: ""
    property string questionDescription: ""
    property string answerDescription: ""

    property int defaultWidth: 112*scale
    property int defaultHeight: 72*scale

    width: defaultWidth*Math.min(Window.width, Window.height)/500
    height: defaultHeight*Math.min(Window.width, Window.height)/500
    color: "white"

    property int textYOffset: height/4
    property int textXOffset: width/13

    Text {
        id: topText
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.textYOffset
        width: parent.width - 2*parent.textXOffset
        height:  parent.height/2
        text: root.question
        font.family: "Arial"
        font.pointSize: Math.floor(11 * Window.width/2200 + 1)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: bottomText
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height*2.5/3.5
        width: parent.width - 2*parent.textXOffset
        height: parent.height - y - parent.textYOffset/3
        text: root.questionDescription
        font.family: "Arial"
        font.pointSize: Math.floor(9*Window.width/2200 + 1)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            topText.text = (topText.text === root.question ? root.answer : root.question)
            bottomText.text = (bottomText.text === root.questionDescription ? root.answerDescription : root.questionDescription)
        }
    }
}
