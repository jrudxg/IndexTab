import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 3840
    height: 2160
    visible: true
    title: qsTr("Hello World")

    visibility: Window.Maximized

    Menubar {
        id: menuBar
        onHomeButtonClicked: pageLoader.source = "HomeScreen.qml"

        onMenuButtonClicked: pageLoader.sourceComponent = menuScreen

        onPlayButtonClicked: pageLoader.sourceComponent = playScreen

        onSettingsButtonClicked: pageLoader.sourceComponent = settingsScreen

    }
    Loader {
        id: pageLoader
        anchors.left: menuBar.right
        anchors.right: parent.right
        height: parent.height
        source:  "HomeScreen.qml"
    }

    Component {
        id: menuScreen
        Rectangle {
            anchors.fill: parent
            color: "gray"
        }
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
