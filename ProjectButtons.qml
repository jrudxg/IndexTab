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

        property int leftMargin: 10
        width: parent.width - 2*leftMargin
        height: parent.width/3
        spacing: 40
        anchors.horizontalCenter: parent.horizontalCenter

        text: name
        font.pointSize: 11 * parent.width / 150

        onClicked: name === "Create Project" ?
        createProjectDialog.open(".txt") :  openProjectDialog.open()
    }

    OpenFolderDialog {
        id: openProjectDialog
        onProjectOpened: (project) => {
            LastProjectModel.openFile(project)
        }
    }

    CreateFileDialog {
        id: createProjectDialog
    }
}

