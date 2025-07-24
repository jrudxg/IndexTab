import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls

import file.manager

Item {

    property bool addToLastProjectModel: true

    // extension needs to begin with .
    function open(fileExtension)  {
        createDialog.extension = fileExtension
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
        title: "Create Project"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true

        property string extension: ".txt"

        width: 500
        height: 500
        anchors.centerIn: Overlay.overlay

        onAccepted: {
            var path = fileManager.createFolder(folderDialog.selectedFolder, textInput.text, true)
            var filePath = fileManager.createFile(path, ("MAIN" + extension))
            LastProjectModel.addNewFile(filePath)
            fileCreated(filePath)
        }


        TextField {
            id: textInput

            width: 500
            height: 175
            anchors.centerIn: parent

            placeholderText: "Enter folder name: "
            onAccepted: createDialog.accept()
        }
    }
}
