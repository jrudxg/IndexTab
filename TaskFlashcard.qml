import QtQuick

Rectangle {
    id: root

    property string question: ""
    property string answer: ""
    property string questionDescription: ""
    property string answerDescription: ""

    property real scale: 1.0

    property real defaultWidth: 168*scale
    property real defaultHeight: 108*scale

    property real defaultX
    property real defaultY


    width: defaultWidth*parent.scaleFactor
    height: defaultHeight*parent.scaleFactor

    x: defaultX*parent.scaleFactor
    y: defaultY*parent.scaleFactor

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
        font.pointSize: Math.floor(6 * root.parent.scaleFactor + 1)
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
        font.pointSize: Math.floor(4 * root.parent.scaleFactor + 1)
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
