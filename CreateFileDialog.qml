import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls

import file.manager

Item {

    property bool addToLastProjectModel: true

    function open() {
        folderDialog.open()
    }

    signal fileCreated(project: string)

    FolderDialog {
        id: folderDialog
        title: "Select folder to save in"
        currentFolder: LastProjectModel.projectsFolderLocation
        onAccepted: createDialog.open()
    }



    FileManager {
        id: fileManager
    }

    Dialog {
        id: createDialog
        title: "Create file"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true

        width: 500
        height: 500
        anchors.centerIn: Overlay.overlay

        onAccepted: {
            var filePath = fileManager.createFile(folderDialog.selectedFolder, (textInput.text + ".txt"))
            if (parent.addToLastProjectModel) {
                LastProjectModel.addNewFile(filePath)
            }
            fileCreated(filePath)
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
