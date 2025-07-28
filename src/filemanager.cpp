#include "include/filemanager.h"
#include <QStringLiteral>
#include <include/projectreader.h>
#include <include/scenedatamodel.h>

FileManager *FileManager::fileManager_Singelton = nullptr;

static ProjectReader *projectReader = nullptr;


const void FileManager::readProjectDir(QString projectDir) {
    QString fileUrl = QStringLiteral(FILE_URL);

    if (projectReader == nullptr) projectReader = new ProjectReader();
    else SceneDataModel::getInstance()->resetData();
    projectReader->readDir(projectDir.remove(fileUrl).chopped(9));
}

QString FileManager::createFolder(QString directory,
                                  QString folderName,
                                  bool shouldCreateImageFolder) const
{

    QString fileUrl = QStringLiteral(FILE_URL);

    QDir dir{directory.remove(fileUrl)};

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
    if (fileUrl == "file://")   {
        return dirPath.mid(1);
    }
    else return dirPath;
}

QString FileManager::getFileUrl() {
    return QStringLiteral(FILE_URL);
}

QString FileManager::createFile(QString directory, QString fileName) const
{

    QString fileUrl = QStringLiteral(FILE_URL);

    QDir dir(directory.remove(fileUrl));

    if (dir.exists(fileName)) return QString();

    QFile file{directory + '/' + fileName};
    file.open(QIODevice::WriteOnly);
    file.close();
    if (fileUrl == "file://")   {
        return dir.path().mid(1) + '/' + fileName;
    }
    else return dir.path() + '/' + fileName;

}

bool FileManager::deleteFolder(QString directory) const
{

    QString fileUrl = QStringLiteral(FILE_URL);

    QDir dir(directory.remove(fileUrl));
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
