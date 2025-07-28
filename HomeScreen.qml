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

    ListView {
        id: content
        width: (parent.width-toolBar.width)/2
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: parent.height/6


        clip: true
        spacing: 15
        interactive: true

        model: LastProjectModel.model

        delegate: Row {
            width: parent.width
            height: 50

            spacing: 5

            Button {
                height: parent.height
                width: height
                onClicked: LastProjectModel.removeFile(model.source)
            }
            ItemDelegate {
                width: parent.width
                height: parent.height
                text: model.name + '\n' + model.source
                onClicked: LastProjectModel.openFile(model.source)
            }
        }
    }


}
