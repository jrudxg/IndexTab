import QtQuick
import QtQuick.Dialogs

Item {

    function open() {fileDialog.open()}

    signal projectOpened(project: string)

    FolderDialog {
        id: fileDialog
        title: "Choose Folder"

        currentFolder: LastProjectModel.projectsFolderLocation + '/' + "Projects"
        onAccepted: {
            projectOpened(selectedFolder)
        }
    }
}




