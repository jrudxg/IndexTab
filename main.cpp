#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "flashcard.h"
#include "folderModel.h"
#include "foldermanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FlashCard>("flashcard.model", 1, 0, "Flashcard");
    qmlRegisterType<FolderModel>("folder.model", 1, 0, "FolderModel");
    qmlRegisterType<FolderManager>("folder.manager", 1, 0, "FolderManager");


    QCoreApplication::setOrganizationDomain("MyCompany");
    QCoreApplication::setOrganizationDomain("https://github.com/AndiFriend/IndexTab");
    QCoreApplication::setApplicationName("IndexTab");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("IndexTab", "Main");


    return app.exec();
}
