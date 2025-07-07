import QtQuick
import Qt.labs.qmlmodels

Item {
    width: 600
    height: 361
    TableView {

        width: parent.width
        height: parent.height

        columnWidthProvider: function (column) {
            return parent.width / columns
        }

        rowHeightProvider: function (row) {
            return parent.height / rows
        }

        model: TableModel {
            TableModelColumn {
                display:  "name"
            }
            TableModelColumn {
                display:  "favAnimal"

            }

            rows: [
                {"name": "John", "favAnimal": "tiger"},
                {"name": "Lisa", "favAnimal": "shark"},
                {"name": "Jack", "favAnimal": "horse"}

            ]
        }
        delegate: Rectangle {
            border.width: 1
            MouseArea {
                anchors.fill: parent
                onClicked: recText.text = "Moin"
            }
            Text {
                id: recText
                text: display
                anchors.centerIn: parent
            }
        }
    }
}
