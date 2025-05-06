import QtQuick
import QtQuick.Controls

ListView {
    id: topList
    anchors.top: parent.top
    anchors.topMargin: 20

    width: parent.width
    height: contentHeight
    interactive: false
    clip: true
    spacing: 8

    model: ListModel {
        ListElement {
            name: "Create Project"
        }
        ListElement {
            name: "Open Project"
        }
    }

    delegate: Button {
        required property string name
        text: name

        property int leftMargin: 30
        width: parent.width - 2*leftMargin
        height: parent.width/3
        anchors.horizontalCenter: parent.horizontalCenter


        onClicked: name === "Create Project" ?
        createFolderDialog.open() :  openFolderDialog.open()
    }

    OpenFolderDialog {
        id: openFolderDialog
        onProjectOpened: (project) => {
            LastProjectModel.openFolder(project)
        }
    }

    CreateFolderDialog {
        id: createFolderDialog
    }
}
