import QtQuick

Rectangle {
    property color backgroundColor: "black"
    id: toolBar
    anchors.left: parent.left
    width: 300 - line.width
    height: parent.height
    color: Qt.darker(backgroundColor, 1.05)

    Rectangle {
        id: line
        anchors.right: parent.right
        width: 2
        height: parent.height
        color: "lightgray"
    }

    ProjectButtons {}

    ToolbarLinks {}
}

