pragma Singleton

import QtCore
import  QtQuick

import  file.model
import file.manager

Item {
    id: lastProjectModel
    property string projectsFolderLocation: settings.projectsLocation
    property string lastEditedProjectsSource: settings.sourceOfLastEditedProject

    signal fileOpened()
    signal fileCreated()


    function setProjectsFolderLocation(selectedFolder) {
        var mainPath = "file:///" + fileManager.createFolder(selectedFolder, "IndexTab", false);
        var path = "file:///" + fileManager.createFolder(mainPath, "Projects", false)
        projectsFolderLocation = mainPath
        settings.projectsLocation = mainPath
    }


    function addNewFile(path)   {

        if (fileModel.addNewFile(path)) {
            settings.sourceOfLastEditedProject = path.replace(fileManager.getFileUrl(), "")
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
        settings.sourceOfLastEditedProject = path.replace(fileManager.getFileUrl(), "") + '/' + "MAIN.txt"
        fileOpened()
    }


    function getText() { return fileModel.getTextFromModelEntry(settings.sourceOfLastEditedProject) }
    function saveText(text) { return fileModel.saveTextToModelEntry(text, settings.sourceOfLastEditedProject) }

    Settings {
        id: settings
        property string projectsLocation: ""
        property string sourceOfLastEditedProject: ""
    }


    FileManager {
        id: fileManager
    }
    property alias model: fileModel
    FileModel {
    id: fileModel

        Component.onCompleted: {

            if (settings.projectsLocation != "") {
                addFromFolderPath(settings.projectsLocation + '/' + "Projects")
            }        
        }
    }
}


