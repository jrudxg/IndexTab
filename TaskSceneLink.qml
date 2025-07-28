import QtQuick
import sceneData.model

Rectangle {
    property string sceneLink
    id: root
    property real scale: 1.0
    property int positionX
    property int positionY

    x: positionX * Window.width/1072
    y: positionY * Window.height/603


    property int defaultWidth: 80*scale
    property int defaultHeight: 45*scale

    width: defaultWidth*Math.min(Window.width, Window.height)/500
    height: defaultHeight*Math.min(Window.width, Window.height)/500

    color: "white"

    Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors.centerIn: parent

        text: parent.sceneLink
        font.pointSize: 20*parent*Window.width/2200
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
