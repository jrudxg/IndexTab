#include "filemanager.h"

FileManager* FileManager::fileManager_Singelton = 0;


QString FileManager::createFolder(QString directory, QString folderName) const {
    QDir dir(directory.remove("file:///"));

    if (dir.exists(folderName)) return QString();

    if(!dir.mkdir(folderName)) return QString();

    return dir.absoluteFilePath(folderName);
}


QString FileManager::createFile(QString directory, QString fileName) const {


    QDir dir(directory.remove("file:///"));

    if (dir.exists(fileName)) return QString();

    QFile file(directory + "/" + fileName);
    file.open(QIODevice::WriteOnly);
    file.close();

    return dir.path() + "/" + fileName;
}

bool FileManager::deleteFile(QString directory) const {
    QFile file(directory.remove("file:///"));
    if (!file.exists()) return false;
    return file.remove();
}

FileManager* FileManager::getInstance() {

    if (!fileManager_Singelton) {
        fileManager_Singelton = new FileManager();
    }

    return fileManager_Singelton;
}
