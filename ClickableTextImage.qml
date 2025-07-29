import QtQuick

Item {
    property int fillMode: Image.Stretch
    property real pointSize: Math.ceil(13 * width / 100)
    required property string imageSource
    required property string textName
    signal buttonPressed

    width: parent.width
    height: image.height + itemText.height
        Image {
            id: image
            anchors.centerIn: parent
            source: parent.imageSource
            fillMode: parent.fillMode
        }

        Text {
            id: itemText
            anchors.top: image.bottom
            anchors.horizontalCenter: image.horizontalCenter
            text:  parent.textName
            font.pointSize: parent.pointSize
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: buttonPressed()
        }
}
