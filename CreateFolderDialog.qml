import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls

import folder.manager

Item {

    property bool addToLastProjectModel: true

    function open() {
        folderDialog.open()
    }

    signal folderCreated(project: string)

    FolderDialog {
        id: folderDialog
        title: "Select folder to save in"
        currentFolder: LastProjectModel.projectsFolderLocation
        onAccepted: createDialog.open()
    }

    FolderManager {
        id: folderManager
    }

    Dialog {
        id: createDialog
        title: "Create folder"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true

        width: 500
        height: 500
        anchors.centerIn: Overlay.overlay

        onAccepted: {
            var folderPath = folderManager.createFolder(folderDialog.selectedFolder, textInput.text)
            if (parent.addToLastProjectModel) {
                LastProjectModel.addNewFolder(folderPath)
            }
            folderCreated(folderPath)
        }

        TextField {
            id: textInput

            width: 500
            height: 175
            anchors.centerIn: parent

            placeholderText: "Enter folder name: "
            onAccepted: createDilaog.accept()
        }
    }
}
