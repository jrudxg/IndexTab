import QtQuick
import sceneData.model
Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: background

        anchors.fill: parent

        color: "lightgray"
    }

    Item {
        id: workableSpace
        anchors {
            fill: parent
            margins: parent.width > parent.height ? parent.width * 0.02 : parent.height * 0.02
        }

        Text {
            anchors {
                right: parent.right
                top: workableSpace.top
            }

            width: implicitWidth
            height:implicitHeight

            font.pointSize: 30

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: SceneDataModel.currentSceneName
        }
    }
}
