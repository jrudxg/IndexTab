import QtQuick
import QtQuick.Controls

Item {
    id: root
    x: 0
    y: 0
    width: 200
    height: parent.height
    Rectangle {
        id: line
        anchors.right: parent.right
        width: 2
        height: parent.height
        color: "lightgrey"
    }

    Rectangle {
        id: background
        y: 0
        anchors.left: parent.left
        anchors.right: line.left
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        height: parent.height
        color: Qt.darker("#2b2929", 1.25)
    }

    signal homeButtonClicked
    signal editButtonClicked
    signal playButtonClicked
    signal settingsButtonClicked

    ListView {
        x: 0
        anchors.top: parent.top
        anchors.topMargin: 40

        width: parent.width
        height: parent.height
        interactive: false
        clip: true
        spacing: 40

        model: ListModel {
            ListElement {
                name: "Home"
                source: "qrc:/images/home.svg"
            }
            ListElement {
                name: "Edit"
                source: "qrc:/images/menu.svg"
            }
            ListElement {
                name: "Play"
                source: "qrc:/images/play.svg"
            }
            ListElement {
                name: "Settings"
                source: "qrc:/images/settings.svg"
            }

        }

        delegate: ClickableTextImage {

            required property string name
            required property string source

            anchors.horizontalCenter: parent.horizontalCenter

            textName: name
            imageSource: source
            fillMode: Image.PreserveAspectFit
            scale: 1.2
            onButtonPressed: {
                name === "Home" ? homeButtonClicked() :
                name === "Edit" ? editButtonClicked() :
                name === "Play" ? playButtonClicked() :
                settingsButtonClicked()
            }
        }
    }
}
