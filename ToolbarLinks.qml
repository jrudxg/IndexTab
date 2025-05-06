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

    model: ListModel {
        ListElement {
            name: "Youtube"
            url: "https://www.youtube.com/"
        }
        ListElement {
            name: "Google"
            url: "https://www.google.de/"
        }
        ListElement {
            name: "Source"
            url: "https://github.com/AndiFriend/IndexTab"
        }
        ListElement {
            name:  "Reddit"
            url: "https://www.reddit.com/"
        }
    }

    delegate: ItemDelegate {
        required property string name
        required property string url
        property int leftMargin: 30
        width: linkText.width
        height: linkText.height
        anchors.horizontalCenter: parent.horizontalCenter
        background: null
        onClicked: Qt.openUrlExternally(url)

        Text {
            id: linkText
            text: name
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.pointSize: 25
            font.underline: true
        }
    }
}
