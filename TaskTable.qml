import QtQuick
import table.model
import QtQuick.Controls
Item {
    property real scale: 1.0
    property string header: ""
    property TaskTableModel model

    id: root

    width: 600 * scale
    height: 361 * scale

    MouseArea {
        anchors.fill: parent
        drag.target: root
    }

    Text {
        id: tableText
        anchors {
            horizontalCenter: tableView.horizontalCenter
            top:  parent.top
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        width: Text.implicitWidth
        height: Text.implicitHeight
        text: root.header

        font.pointSize: 20 * parent.scale
        font.bold: true
    }

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: tableView.left
        anchors.top: tableText.bottom
        syncView: tableView
        clip: true

        interactive: false
    }

    VerticalHeaderView {
        id: verticalHeader
        anchors.top: tableView.top
        anchors.left: parent.left
        syncView: tableView
        clip: true

        interactive: false
    }

    Rectangle {
        anchors {
            top: tableText.bottom
            right: horizontalHeader.left
            bottom: verticalHeader.top
            left:  parent.left
        }
        color: "white"

    }

    TableView {

        interactive: false

        id: tableView

        width: parent.width
        height: parent.height

        anchors.left: verticalHeader.right
        anchors.top: horizontalHeader.bottom

        columnWidthProvider: function (column) {
            return parent.width / columns
        }

        rowHeightProvider: function (row) {
            return parent.height / rows
        }

        model: root.model
        delegate: Rectangle {
        required property string display
            border.width: 1
            MouseArea {
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                onClicked: if (parent.display[0] !== "$") {
                    recText.text = recText.text === parent.display ?  "" : parent.display
                }
            }
            Text {
                id: recText
                anchors.centerIn: parent
                text: parent.display[0] === "$" ? parent.display.substring(1,parent.display.length) : ""

            }
        }
    }
}
