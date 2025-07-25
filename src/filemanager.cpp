#include "include/filemanager.h"

FileManager *FileManager::fileManager_Singelton = 0;

QString FileManager::createFolder(QString directory,
                                  QString folderName,
                                  bool shouldCreateImageFolder) const
{
    // only removing 2 slashes, so the path keeps beeing absolute
    QDir dir = directory.remove("file://");

    if (dir.exists(folderName)) {
        return QString();
    }


    if (!dir.mkdir(folderName)) {
        return QString();
    }


    if (shouldCreateImageFolder) {
        QDir project{dir.absoluteFilePath(folderName)};
        if (!project.mkdir("images"))
            return QString();
    }
    QString dirPath = dir.absoluteFilePath(folderName);
    return dirPath.last(dirPath.size()-1);
}

QString FileManager::createFile(QString directory, QString fileName) const
{
    QDir dir(directory.remove("file://"));

    if (dir.exists(fileName)) return QString();

    QFile file{directory + '/' + fileName};
    file.open(QIODevice::WriteOnly);
    file.close();

    return dir.path().mid(1) + '/' + fileName;
}

bool FileManager::deleteFolder(QString directory) const
{
    QDir dir(directory.remove("file://"));
    if (!dir.exists()) {
        return false;
    }
    return dir.removeRecursively();
}

FileManager *FileManager::getInstance()
{
    if (!fileManager_Singelton) {
        fileManager_Singelton = new FileManager();
    }

    return fileManager_Singelton;
}
