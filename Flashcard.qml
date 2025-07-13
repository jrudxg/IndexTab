import QtQuick

import utility.flashcard

Rectangle {
    id: root

    property string question: ""
    property string answer: ""
    property string questionDescription: ""
    property string answerDescription: ""

    // approximately 14:9
    width: 400
    height: 257
    color: "white"

    property int textYOffset: height/6
    property int textXOffset: width/13

    MouseArea {
        anchors.fill: parent
        drag.target: root
    }

    Text {
        id: topText
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.textYOffset
        width: parent.width - 2*parent.textXOffset
        height:  parent.height/2
        text: root.question
        font.family: "Arial"
        font.pointSize: 11
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
        font.pointSize: 9
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            topText.text = (topText.text === root.question ? FlashcardUtil.getcAnswerFromuAnswer(root.answer) : root.question)
            bottom.text = (bottomText.text === root.questionDescription ? root.answerDescription : root.questionDescription)
        }
    }
}
