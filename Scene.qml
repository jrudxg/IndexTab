import QtQuick
import sceneData.model
Item {
    id: root

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

        TextMetrics {
            id: t_metricts
            text: text.text
            font: text.font
        }

        Text {
            id: text
            anchors {
                top: workableSpace.top
                right: workableSpace.right
            }
            width: t_metricts.tightBoundingRect.width
            height: t_metricts.tightBoundingRect.height

            font.pointSize: Math.floor(11 * root.width / 1325 + 1)

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: SceneDataModel.currentSceneName
        }
    }
}
