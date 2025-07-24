import QtQuick
import QtQuick.Dialogs

Item {

    function open(customNameFiltes) {fileDialog.open()}

    signal projectOpened(project: string)


    FolderDialog {
        id: fileDialog
        title: "Choose Folder"

        currentFolder: LastProjectModel.projectsFolderLocation

        onAccepted: {
            projectOpened(selectedFolder)
        }
    }
}




