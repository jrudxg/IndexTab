#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QSettings>

#include <QQuickWebEngineProfile>
#include <QtWebEngineQuick/qtwebenginequickglobal.h>

#include "include/filemodel.h"
#include "include/filemanager.h"
#include "include/projectreader.h"
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

    qmlRegisterSingletonInstance<SceneDataModel>("sceneData.model",
                                                 1,
                                                 0,
                                                 "SceneDataModel",
                                                 new SceneDataModel(engine));

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
            else filePath = settings.value("sourceOfLastEditedProject").toString().remove(fileUrl).chopped(9);
        }

    }

    ProjectReader reader;
    reader.readDir(filePath);

    QQuickWebEngineProfile *profile = QQuickWebEngineProfile::defaultProfile();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        QTimer::singleShot(0, []() { delete QQuickWebEngineProfile::defaultProfile(); });
    });

    engine.loadFromModule("IndexTab", "Main");
    return app.exec();
}
