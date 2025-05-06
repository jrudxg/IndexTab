#include "foldermanager.h"

FolderManager* FolderManager::folderManager_Singelton = 0;

FolderManager::FolderManager(QObject *parent)
    : QObject(parent) {}

QString FolderManager::createFolder(QString directory, QString folderName) const {


    QDir dir(directory.remove("file:///"));

    if (dir.exists(folderName)) return QString();

    if (!dir.mkdir(folderName)) return QString();

    return dir.absoluteFilePath(folderName);
}

bool FolderManager::deleteFolder(QString directory) const {

    QDir dir(directory.remove("file:///"));
    if (!dir.exists()) return false;
    return dir.removeRecursively();
}

FolderManager* FolderManager::getInstance() {

    if (!folderManager_Singelton) {
        folderManager_Singelton = new FolderManager();
    }

    return folderManager_Singelton;
}
