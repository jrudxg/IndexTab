import QtQuick
import QtQuick.Dialogs

Item {

    function open(customNameFiltes) {
        fileDialog.nameFilters = customNameFiltes
        fileDialog.open()
    }

    signal projectOpened(project: string)


    FileDialog {
        id: fileDialog
        title: "Ordner auswählen"
        nameFilters: ["All files (*)"]

        currentFolder: LastProjectModel.projectsFolderLocation

        onAccepted: {
            projectOpened(selectedFile)
        }
    }
}




