import QtQuick
import QtQuick.Controls
import flashcard.model

Item {
    id: root

    // 1. Modell einmalig erstellen
    Flashcard {
        id: flashcardModel
    }

    // 2. SwipeView verwenden
    SwipeView {
        id: swipeView
        anchors.fill: parent

        clip: true

        Repeater {
            model: flashcardModel

            Page {
                implicitWidth: swipeView.width
                implicitHeight: swipeView.height

                Label {
                    id: label
                    anchors.fill: parent
                    text: model.question
                    font.pixelSize: 25
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: label.text === model.question
                        ? label.text = model.answer
                        : label.text = model.question
                }
            }
        }
    }
}
