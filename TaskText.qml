// Copyright 2015 The Chromium Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google LLC nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
        Markdown,
        None
    }

    signal loadedProperties
    property int requiredPropertiesLoaded: 0

    onInputChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded == 3)
        loadedProperties()
    }
    onImagePathChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded == 3)
        loadedProperties()
    }
    onTextTypeChanged: {
        requiredPropertiesLoaded++
        if (requiredPropertiesLoaded == 3)
        loadedProperties()
    }

    property string input: ""
    property string imagePath: ""
    property int textType: TaskText.TextTypeEnum.None

    property real scale: 1

    property real defaultX
    property real defaultY

    property real defaultWidth: 400*scale
    property real defaultHeight: 400*scale


    x: defaultX * parent.scaleFactor
    y: defaultY * parent.scaleFactor

    width: defaultWidth * parent.scaleFactor
    height: defaultHeight * parent.scaleFactor


    onLoadedProperties: {
        root.textType === TaskText.TextTypeEnum.Markdown ? loader.sourceComponent = webEngineView : loader.sourceComponent = textField;
    }


    Loader {
        id: loader
        anchors.fill: parent
    }

    Component {
        id: textField
        Text {
            function nthIndex(str, pat, n){
                var L= str.length, i= -1;
                while(n-- && i++<L){
                    i= str.indexOf(pat, i);
                    if (i < 0) break;
                }
                return i;
            }

            anchors.fill: parent
            text: root.input.trim()
            textFormat: Text.PlainText
            font.pointSize: nthIndex(root.input.trim(), '\n', 3) === -1 ? Math.floor(10 * root.parent.scaleFactor + 1) : Math.floor(11 * root.parent.scaleFactor + 1)
            font.bold: nthIndex(root.input.trim(), '\n', 3) === -1
        }
    }

    FileManager {
        id: fileManager
    }

    Component {
        id: webEngineView
        WebEngineView {
            id: view
            anchors.fill: parent
            zoomFactor: root.parent.scaleFactor/1.5
            focus: true

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
