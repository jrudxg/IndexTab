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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QSettings>

#include <QQuickWebEngineProfile>
#include <QtWebEngineQuick/qtwebenginequickglobal.h>

#include "include/filemodel.h"
#include "include/filemanager.h"
#include "include/scenedatamodel.h"
#include "include/tablemodel.h"

int main(int argc, char *argv[])
{
    QString fileUrl = QStringLiteral(FILE_URL);

    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setOrganizationDomain("https://github.com/AndiFriend/IndexTab");
    QCoreApplication::setApplicationName("IndexTab");

    QtWebEngineQuick::initialize();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<FileModel>("file.model", 1, 0, "FileModel");
    qmlRegisterType<FileManager>("file.manager", 1, 0, "FileManager");

    qmlRegisterType<TableModel>("table.model", 1, 0, "TaskTableModel");

    qmlRegisterSingletonInstance<SceneDataModel>("sceneData.model", 1, 0, "SceneDataModel", new SceneDataModel(engine));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    QSettings settings;
    QString filePath{};
    {
        QDir projectDir{settings.value("projectsLocation").toString().remove(fileUrl)};

        if (!projectDir.isReadable()) {
            settings.remove("projectsLocation");
            settings.remove("sourceOfLastEditedProject");
        }
        else {
            QFile lastEditedProject{settings.value("sourceOfLastEditedProject").toString().remove(fileUrl)};
            if (!lastEditedProject.exists()) settings.remove("sourceOfLastEditedProject");
            else filePath = settings.value("sourceOfLastEditedProject").toString().remove(fileUrl);
        }

    }

    FileManager::readProjectDir(filePath);


    QQuickWebEngineProfile *profile = QQuickWebEngineProfile::defaultProfile();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        QTimer::singleShot(0, []() { delete QQuickWebEngineProfile::defaultProfile(); });
    });

    engine.loadFromModule("IndexTab", "Main");
    return app.exec();
}
