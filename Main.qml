import QtQuick
import QtCore
import QtQuick.Controls
import sceneData.model
import QtWebEngine
import Qt.labs.platform as Platform
import QtQuick.Dialogs
import file.manager

ApplicationWindow {
    visible: true
    title: qsTr("IndexTab")

    width: 1072
    height: 603

    Menubar {
        id: menuBar

        x: 0
        y: 0

        onHomeButtonClicked: {
            if (LastProjectModel.projectsFolderLocation === "") {
                folderDialog.open()
            }
            pageLoader.source = "HomeScreen.qml"
        }
        function loadEditScreenWhenFileExists() {
            if (LastProjectModel.lastEditedProjectsSource === "")
                pageLoader.sourceComponent = noFileEditScreen
            else
                pageLoader.sourceComponent = editScreen
        }

        Connections {
            target: LastProjectModel
            function onFileOpened() {
                pageLoader.sourceComponent = editScreen
            }
            function onFileCreated() {
                pageLoader.sourceComponent = editScreen
            }
        }

        onEditButtonClicked: loadEditScreenWhenFileExists()

        onPlayButtonClicked: pageLoader.sourceComponent = playScreen
        onSettingsButtonClicked: pageLoader.sourceComponent = settingsScreen
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Choose folderpath to create project folder")

        currentFolder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.HomeLocation)

        onAccepted: {
            LastProjectModel.setProjectsFolderLocation(selectedFolder)
        }
    }


    Loader {
        id: pageLoader

        anchors.left: menuBar.right
        width: parent.width - menuBar.width
        height: parent.height
        Component.onCompleted: {
            menuBar.homeButtonClicked()
        }
    }

    Component {
        id: noFileEditScreen
        Rectangle {
            anchors.fill: parent

            color: "white"

            Text {
                anchors.centerIn: parent
                text: qsTr("Please open or create a file")
                font.pointSize: 25
            }
        }
    }



    Component {
        id: editScreen

        Rectangle {

            anchors.fill: parent
            color: "white"

            Component.onDestruction: {
                LastProjectModel.saveText(textEdit.text)
                LastProjectModel.saveText(textEdit.text)
            }


            Flickable {
                 id: flick

                 width:parent.width;
                 height: parent.height;
                 contentWidth: textEdit.contentWidth
                 contentHeight: textEdit.contentHeight
                 clip: true

                 function ensureVisible(r)
                 {
                     if (contentX >= r.x)
                         contentX = r.x;
                     else if (contentX+width <= r.x+r.width)
                         contentX = r.x+r.width-width;
                     if (contentY >= r.y)
                         contentY = r.y;
                     else if (contentY+height <= r.y+r.height)
                         contentY = r.y+r.height-height;
                 }

                TextEdit {
                    id: textEdit
                    text: LastProjectModel.getText()

                    wrapMode: Text.Wrap
                    onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

                    Component.onCompleted: {
                        focus = true
                    }


                    Connections {
                     target: LastProjectModel

                     function onFileOpened() {
                         text = LastProjectModel.getText()
                     }

                     function onFileCreated() {
                         text = LastProjectModel.getText()
                     }
                    }

                    width: flick.width

                    font.pointSize: 25
                }

             }

            Text {
                anchors {
                    right: parent.right
                    rightMargin: 5
                    top: parent.top
                }

                text: LastProjectModel.lastEditedProjectsSource
                font.pointSize: 11  * Window.width / 1325
                color: Qt.lightGray
            }

            Button {
                text: qsTr("save")
                onClicked: {
                    LastProjectModel.saveText(textEdit.text)
                }

                width: 300
                height: 100

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: 5
                    bottomMargin: 5
                }
            }
        }
    }

    Component {
        id: playScreen
        Loader {
            data: SceneDataModel.currentScene
        }
    }

    Component {
        id: settingsScreen

        Rectangle {

            color: Qt.lighter("lightgray", 1.1)
            anchors.fill: parent

            ListModel {
                id: settingsModel
                ListElement {
                    name:  "Last edited project source:"
                    setting: 1
                }
                ListElement {
                    name:  "Project location:"
                    setting: 2
                }
            }

            FileManager {
                id: fileManager
            }

            ListView {
                anchors.fill: parent
                model: settingsModel
                spacing: 5 * Window.height / 1000
                delegate: Row {
                    required property string name
                    required property int setting
                    readonly property string settingValue: setting === 1 ? LastProjectModel.lastEditedProjectsSource.replace(fileManager.getFileUrl(), "")
                                                                                                                    .substring(0, LastProjectModel.lastEditedProjectsSource.length - 9)
                                                                         : LastProjectModel.projectsFolderLocation.replace(fileManager.getFileUrl(), "")
                    Text {id: text; text: name; font.pointSize: 11 * Window.width / 1000}
                    TextEdit {
                        id: edit
                        text: settingValue;
                        font.pointSize: 11 * Window.width / 1000

                        Component.onCompleted: {
                            if (width < 100) width = 100
                        }

                        onWidthChanged: {
                            if (width < 100) width = 100
                        }

                        onImplicitHeightChanged: {
                            if (implicitWidth > 100) width = implicitWidth
                        }
                    }

                    spacing: 5 * Window.width / 1000

                    Component.onDestruction: {
                        if (setting === 1) {LastProjectModel.openFile(edit.text)}
                        if (setting === 2) LastProjectModel.setSettings(edit.text, "")
                    }
                }
            }
        }
    }
}
