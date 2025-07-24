#include <QTimer>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#include <QQuickWebEngineProfile>

#include "fileModel.h"
#include "filemanager.h"
#include "projectreader.h"
#include "scenedatamodel.h"
#include "tablemodel.h"

int main(int argc, char *argv[])
{
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

    QString projectPath = "C:/Users/afreu/Documents/Projects/B";
    QString imagePath = projectPath + "/images";


    ProjectReader reader;
    reader.readDir("C:/Users/afreu/Documents/Projects/B");

    QQuickWebEngineProfile* profile = QQuickWebEngineProfile::defaultProfile();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        QTimer::singleShot(0, []() {
            delete QQuickWebEngineProfile::defaultProfile();
        });
    });

    engine.load(QUrl(QStringLiteral("IndexTab/Main.qml")));

    return app.exec();
}
