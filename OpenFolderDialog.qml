import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import Qt.labs.folderlistmodel

Item {

    function open() {
        folderDialog.open()
    }

    signal projectOpened(project: string)


    FolderDialog {
        id: folderDialog
        title: "Ordner auswählen"
        currentFolder: LastProjectModel.projectsFolderLocation

        onAccepted: {
            projectOpened(selectedFolder)
        }
    }
}




