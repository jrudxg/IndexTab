import QtQuick
import QtWebEngine
import sceneData.model
import file.manager

Item {
    id: root

    /*
       plainText is for when you just want text without any markdown
       Markdown uses markdown-it (https://github.com/markdown-it/markdown-it) with many plugins and renders the text using WebEngineView
    */
    enum TextTypeEnum {
        PlainText,
        Markdown
    }

    signal loadedProperties
    property int requiredPropertiesLoaded: 0

    onInputChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded === 3)
        loadedProperties()
    }
    onImagePathChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded === 3)
        loadedProperties()
    }
    onTextTypeChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded === 3)
        loadedProperties()
    }



    property string input: ""
    property string imagePath: ""
    property int textType: TaskText.TextTypeEnum.PlainText
    property real scale: 1

    width: 800*scale
    height: 800*scale

    onLoadedProperties: {
        root.textType === TaskText.TextTypeEnum.Markdown ? loader.sourceComponent = webEngineView : loader.sourceComponent = textField
    }


    Loader {
        id: loader
        anchors.fill: parent

    }

    Component {
        id: textField
        Text {
            anchors.fill: parent
            text: root.input
            textFormat: Text.PlainText
            font.pointSize: 20
        }
    }

    FileManager {
        id: fileManager
    }

    Component {
        id: webEngineView
        WebEngineView {
            anchors.fill: parent
            Component.onCompleted: {
                loadHtml(root.input, fileManager.getFileUrl() + root.imagePath)
            }



            onNavigationRequested: function(request) {
                var url = request.url.toString()
                if (url.startsWith("scene://")) {
                    request.reject()
                    SceneDataModel.setCurrentScenePerName(url.substring(8))
                } else {
                    request.accept()
                }
            }
        }
    }
}
