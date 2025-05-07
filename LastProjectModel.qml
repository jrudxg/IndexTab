pragma Singleton

import QtCore
import  QtQuick
import QtQuick.Controls
import Qt.labs.platform as Platform
import QtQuick.Dialogs

import  folder.model
import folder.manager

Item {
    id: lastProjectModel
    property string projectsFolderLocation: settings.projectsLocation
    anchors.centerIn: Overlay.overlay

    function addNewFolder(path) { return folderModel.addNewFolder(path) }
    function removeFolder(path) { return folderModel.removeFolder(path) }
    function openFolder(path)   { return folderModel.openFolder(path) }

    Settings {
        id: settings
        property string projectsLocation: ""
    }


    FolderDialog {
        id: folderDialog
        title: "Ordner auswählen"
        currentFolder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        onAccepted: {
            settings.projectsLocation = "file:///" + folderManager.createFolder(selectedFolder, "Projects")
            lastProjectModel.projectsFolderLocation = settings.projectsLocation
        }
    }

    FolderManager {
        id: folderManager
    }

    property alias model: folderModel

    FolderModel {
    id: folderModel

        Component.onCompleted: {
            if (!folderManager.doesFileExist(settings.projectsLocation)) folderDialog.open()
            addFromFolderPath(settings.projectsLocation)
        }
    }
}


