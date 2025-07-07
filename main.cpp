#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "fileModel.h"
#include "filemanager.h"
#include "projectreader.h"
#include "scenedatamodel.h"
#include "flashcardUtility.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;


    qmlRegisterType<FileModel>("file.model", 1, 0, "FileModel");
    qmlRegisterType<FileManager>("file.manager", 1, 0, "FileManager");

    FlashcardUtility* flashcardUtility = new FlashcardUtility();
    qmlRegisterSingletonInstance<FlashcardUtility>("utility.flashcard", 1, 0, "FlashcardUtil", flashcardUtility);

    SceneDataModel* dataModel = new SceneDataModel(engine);
    qmlRegisterSingletonInstance<SceneDataModel>("sceneData.model", 1, 0, "SceneDataModel", dataModel);




    QCoreApplication::setOrganizationDomain("MyCompany");
    QCoreApplication::setOrganizationDomain("https://github.com/AndiFriend/IndexTab");
    QCoreApplication::setApplicationName("IndexTab");



    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("IndexTab/Main.qml")));

    ProjectReader reader;
    reader.readFile("file:///C:/Users/afreu/Documents/Projects/B.txt");

    return app.exec();
}
