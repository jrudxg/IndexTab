import QtQuick
import sceneData.model

Rectangle {
    property string sceneLink
    id: root

    width: 140
    height: 90
    color: "white"

    Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors.centerIn: parent

        text: parent.sceneLink
        font.pointSize: 20
        font.underline: true
        color: "blue"
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            SceneDataModel.setCurrentScenePerName(sceneLink)
        }
    }
}
