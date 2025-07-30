import QtQuick
import sceneData.model

Rectangle {
    id: root

    property string sceneLink
    property real scale: 1.0

    property real defaultWidth: 120*scale
    property real defaultHeight: 68*scale

    property real defaultX
    property real defaultY


    width: defaultWidth*parent.scaleFactor
    height: defaultHeight*parent.scaleFactor

    x: defaultX*parent.scaleFactor
    y: defaultY*parent.scaleFactor

    color: "white"

    Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors.centerIn: parent

        text: parent.sceneLink
        font.pointSize: Math.floor(9 * root.parent.scaleFactor + 1)
        font.underline: true
        color: "blue"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            SceneDataModel.setCurrentScenePerName(sceneLink)
        }
    }
}
