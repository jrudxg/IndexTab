import QtQuick
import table.model
import QtQuick.Controls
Item {
    id: root

    property real scale: 1.0
    property string header: ""
    property TaskTableModel model


    property real defaultWidth: 375*scale
    property real defaultHeight: 225*scale

    property real defaultX
    property real defaultY


    width: defaultWidth*parent.scaleFactor
    height: defaultHeight*parent.scaleFactor

    x: defaultX*parent.scaleFactor
    y: defaultY*parent.scaleFactor


    signal resetRequested

    Text {
        id: tableText
        anchors {
            horizontalCenter: tableView.horizontalCenter
            top:  root.top
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        width: tableView.width
        height: Text.implicitHeight
        text: root.header
        font.pointSize: Math.floor(9 * root.parent.scaleFactor + 1)
        font.bold: true
    }

    Text {
        id: resetButton
        anchors {
            top: root.top
            right: tableView.right
            rightMargin: 5 * root.parent.scaleFactor
        }
        text: "↻"
        font.pointSize: Math.floor(11 * root.parent.scaleFactor + 1)

        MouseArea {
            anchors.fill: parent
            onClicked: root.resetRequested()
        }
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
            required property int row
            required property int column

            color: (row+column) % 2 === 0 ? "white" : Qt.lighter("lightgray", 1.15)

            MouseArea {
                anchors.fill: parent
                onClicked: if (parent.display[0] !== '$') {
                    if (recText.text === parent.display) {
                        parent.color = (row+column) % 2 === 0 ? "white" : Qt.lighter("lightgray", 1.15)
                        recText.text = ""
                        button_correct.text = ""
                    } else {
                        recText.text = parent.display
                        button_correct.text = '✔'
                    }
                }
            }

            Text {
                id: button_correct

                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    rightMargin: 5 * root.parent.scaleFactor
                }

                width: implicitWidth
                height: implicitHeight

                font.pointSize: Math.floor(5 * root.parent.scaleFactor + 1)

                MouseArea {
                    anchors.fill: parent
                    onClicked: if (parent.parent.display[0] !== '$' && recText.text === parent.parent.display){
                        if (parent.text === '←') {
                            parent.text = '✔'
                            parent.parent.color = (parent.parent.row + parent.parent.column) % 2 === 0 ? "white" : Qt.lighter("lightgray", 1.15)
                        }
                        else  {
                            parent.parent.color = "green"
                            parent.text = '←'
                        }
                    }
                }
            }

            Connections {
                target: root
                function onResetRequested() {
                    recText.text = ""
                    button_correct.text = ""
                    color = (row+column) % 2 === 0 ? "white" : Qt.lighter("lightgray", 1.15)
                }
            }

            Text {
                id: recText
                anchors.fill: parent
                font.pointSize: Math.floor(4 *root.parent.scaleFactor + 1)
                text: parent.display[0] === "$" ? parent.display.substring(1,parent.display.length) : ""
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin:  3 * root.parent.scaleFactor
            }
        }
    }
}
