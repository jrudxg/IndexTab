import QtQuick
import QtQuick.Controls

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
        width:  200
        height: parent.height

        onHomeButtonClicked: pageLoader.source = "HomeScreen.qml"

        function loadEditScreenWhenFileExists() {
            if (LastProjectModel.lastEditedProjectsSource === "") pageLoader.sourceComponent = noFileEditScreen
            else pageLoader.sourceComponent = editScreen
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
        source:  "HomeScreen.qml"
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

            TextEdit {
                id: textEdit
                text: LastProjectModel.getText()

                Connections {
                    target:  LastProjectModel

                    function onFileOpened() {
                        text = LastProjectModel.getText()
                    }

                    function onFileCreated() {
                        text = LastProjectModel.getText()
                    }
                }

                anchors {
                    fill: parent
                    margins: 20
                }

                font.pointSize: 25
            }

            Text {
                anchors {
                    right: parent.right
                    top:  parent.top
                }

                text:  LastProjectModel.lastEditedProjectsSource
                font.pointSize: 11
                color: Qt.lightGray
            }

            Shortcut {
                sequence:  "Ctrl+S"
                onActivated: LastProjectModel.saveText(textEdit.text)
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
                    bottom:  parent.bottom
                    rightMargin: 5
                    bottomMargin: 5
                }
            }
        }
    }

Component {
    id: textFileEditor

}

    Component {
        id: playScreen
        Rectangle {
            anchors.fill: parent
            color: Qt.lighter("gray", 1.1)
        }
    }

    Component {
        id: settingsScreen
        Rectangle {
            anchors.fill: parent
            color: Qt.lighter("gray", 1.2)
        }
    }
}
