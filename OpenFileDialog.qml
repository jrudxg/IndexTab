import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import Qt.labs.folderlistmodel

Item {

    function open() {
        fileDialog.open()
    }

    signal projectOpened(project: string)


    FileDialog {
        id: fileDialog
        title: "Ordner auswählen"
        nameFilters: ["Text files (*.txt)"]
        currentFolder: LastProjectModel.projectsFolderLocation

        onAccepted: {
            projectOpened(selectedFile)
        }
    }
}




