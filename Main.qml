import QtQuick
import QtQuick.Controls
import sceneData.model
import QtWebEngine

Window {
    width: 3840
    height: 2160
    visible: true
    title: qsTr("Hello World")

    visibility: Window.Maximized

    Menubar {
        id: menuBar

        x: 0
        y: 0
        width: 200
        height: parent.height

        onHomeButtonClicked: pageLoader.source = "HomeScreen.qml"

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

        onEditButtonClicked: pageLoader.sourceComponent = editScreen

        onPlayButtonClicked: pageLoader.sourceComponent = playScreen
        onSettingsButtonClicked: pageLoader.sourceComponent = settingsScreen
    }

    Loader {
        id: pageLoader
        anchors.left: menuBar.right
        width: parent.width - menuBar.width
        height: parent.height
        source: "HomeScreen.qml"
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


            Flickable {
                 id: flick

                 width: parent.width; height: parent.height;
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

                     focus: true
                     wrapMode: Text.Wrap
                     onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

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
                font.pointSize: 11
                color: Qt.lightGray
            }

            Shortcut {
                sequence: "Ctrl+S"
                onActivated: {
                    LastProjectModel.saveText(textEdit.text)
                }
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

        }
    }
}
