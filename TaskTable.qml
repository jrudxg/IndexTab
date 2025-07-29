import QtQuick
import table.model
import QtQuick.Controls
Item {
    property real scale: 1.0
    property string header: ""
    property TaskTableModel model

    property int positionX
    property int positionY

    x: positionX * Window.width/1072
    y: positionY * Window.height/603


    id: root

    property int defaultWidth: 300*scale
    property int defaultHeight: 180*scale

    width: defaultWidth*Math.min(Window.width, Window.height)/500
    height: defaultHeight*Math.min(Window.width, Window.height)/500

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
        font.pointSize: 20 * Window.width/2200
        font.bold: true
    }

    Text {
        id: resetButton
        anchors {
            top: root.top
            right: root.right
        }
        text: "↻"
        font.pointSize: 23 * Window.width/2200
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

            Button {
                id: button_correct
                onClicked: if (parent.display[0] !== '$' && recText.text === parent.display){
                    if (text === '←') {
                        text = '✔'
                        parent.color = (row+column) % 2 === 0 ? "white" : Qt.lighter("lightgray", 1.15)
                    }
                    else  {
                        parent.color = "green"
                        text = '←'
                    }
                }
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    rightMargin: 10 * Window.width/2200
                }

                width: implicitWidth
                height: implicitHeight

                font.pointSize: 13 * Window.width/2200
                background: Qt.transparent
                Component.onCompleted: {
                }
            }

            Connections {
                target: root
                onResetRequested: {
                    recText.text = ""
                    button_correct.text = ""
                    color = "white"
                }
            }

            Text {
                id: recText
                anchors.fill: parent
                font: 7 * Window.width/2200
                text: parent.display[0] === "$" ? parent.display.substring(1,parent.display.length) : ""
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin:  5 * Window.width/2200
            }
        }
    }
}
