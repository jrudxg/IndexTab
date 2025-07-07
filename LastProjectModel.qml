pragma Singleton

import QtCore
import  QtQuick
import QtQuick.Controls
import Qt.labs.platform as Platform
import QtQuick.Dialogs

import  file.model
import file.manager

Item {
    id: lastProjectModel
    property string projectsFolderLocation: settings.projectsLocation
    property string lastEditedProjectsSource: settings.sourceOfLastEditedProject

    signal fileOpened()
    signal fileCreated()


    function addNewFile(path)   {

        if (fileModel.addNewFile(path)) {
            settings.sourceOfLastEditedProject = path
            fileCreated()
            return true
        }
        return false
    }

    function removeFile(path) {
        if(fileModel.removeFile(path)) {
            settings.sourceOfLastEditedProject = ""
            return true
        }
        return false
    }

    function openFile(path) {
        settings.sourceOfLastEditedProject = path
        fileOpened()
    }

    function getText() { return fileModel.getTextFromModelEntry(settings.sourceOfLastEditedProject) }
    function saveText(text) { return fileModel.saveTextToModelEntry(text, settings.sourceOfLastEditedProject) }

    Settings {
        id: settings
        property string projectsLocation: ""
        property string sourceOfLastEditedProject: ""
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Choose folderpath to create folder")
        currentFolder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        onAccepted: {
            settings.projectsLocation = "file:///" + fileManager.createFolder(selectedFolder, "Projects")
            lastProjectModel.projectsFolderLocation = settings.projectsLocation
        }
    }

    FileManager {
        id: fileManager
    }
    property alias model: fileModel
    FileModel {
    id: fileModel

        Component.onCompleted: {
            if (!fileManager.doesFolderExist(settings.projectsLocation)) folderDialog.open()
            addFromFolderPath(settings.projectsLocation)
        }
    }
}


