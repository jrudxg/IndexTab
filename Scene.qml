import QtQuick
import sceneData.model

Rectangle {
    id: root
    anchors {
        top: parent.top
        left: parent.left
    }

    color: "lightgray"
    property real scaleFactor: Math.min(parent.width / baseWidth, parent.height / baseHeight)

    readonly property real baseWidth: 1000.5333
    readonly property real baseHeight: 603

    width: baseWidth * scaleFactor
    height: baseHeight * scaleFactor

    TextMetrics {
        id: t_metricts
        text: text.text
        font: text.font
    }

    Text {
        id: text
        anchors {
            top: parent.top
            right: parent.right
            margins: parent.width > parent.height ? parent.width * 0.02 : parent.height * 0.02
        }
        width: t_metricts.tightBoundingRect.width
        height: t_metricts.tightBoundingRect.height

        font.pointSize: Math.floor(11 * parent.width / baseWidth + 1)

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: SceneDataModel.currentSceneName
    }
}
