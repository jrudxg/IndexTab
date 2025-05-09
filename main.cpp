#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtGui>
#include <QtCore>

#include "flashcard.h"
#include "fileModel.h"
#include "filemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FlashCard>("flashcard.model", 1, 0, "Flashcard");
    qmlRegisterType<FileModel>("file.model", 1, 0, "FileModel");
    qmlRegisterType<FileManager>("file.manager", 1, 0, "FileManager");


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
    engine.load(QUrl(QStringLiteral("IndexTab/Main.qml")));


    return app.exec();
}
