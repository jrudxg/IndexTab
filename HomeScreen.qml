import QtQuick
import QtQuick.Controls
import QtQml.Models

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: background
        anchors.left: toolBar.right
        anchors.right: parent.right
        height: parent.height
        color: "#2b2929"
    }

    HomeToolbar {
        id: toolBar
        backgroundColor: background.color
    }

    FileFolder {
        x: 1000
        y: 1000
        width: 100
        height: 100
    }

    ListView {
        id: content
        width: 1500
        height: 1200
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 210

        clip: true
        spacing: 15
        interactive: true

        model: LastProjectModel.model

        delegate: Row {
            width: 1500
            height: 50

            spacing: 5

            Button {
                height: parent.height
                width: height
                onClicked: LastProjectModel.removeFolder(model.source)
            }
            ItemDelegate {
                width: parent.width
                height: parent.height
                text: model.name + '\n' + model.source
                onClicked: LastProjectModel.openFolder(model.source)
            }
        }
    }

}
