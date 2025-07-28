import QtQuick
import QtQuick.Controls

ListView {
    id: bottomList
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    width: parent.width
    height: contentHeight
    clip: true
    spacing: 15

    Window {
        id: window
        width: 500
        height: 500
        TextArea {
            textFormat: TextArea.MarkdownText
            text:  '
# Guide
is still in progress
'
        }
    }

    model: ListModel {
        ListElement {
            name: "Guide"
            url: "https://www.youtube.com/"
            shouldLoadLocally: true
        }
        ListElement {
            name: "Discord"
            url: "https://discord.gg/MzsHmDbQnN"
            shouldLoadLocally: false
        }
        ListElement {
            name: "Source"
            url: "https://github.com/AndiFriend/IndexTab"
            shouldLoadLocally: false
        }
        ListElement {
            name:  "Reddit"
            url: "https://www.reddit.com/"
            shouldLoadLocally: false
        }
    }

    delegate: ItemDelegate {
        required property string name
        required property string url
        required property bool shouldLoadLocally
        property int leftMargin: 30
        width: linkText.width
        height: linkText.height
        anchors.horizontalCenter: parent.horizontalCenter
        background: null
        onClicked: shouldLoadLocally ? window.show() : Qt.openUrlExternally(url)

        Text {
            id: linkText
            text: name
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.pointSize: 11 * bottomList.width / 100
            font.underline: true
        }
    }
}
