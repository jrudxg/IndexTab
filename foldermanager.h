#ifndef FOLDERMANAGER_H
#define FOLDERMANAGER_H

#include <QtCore>

class FolderManager : public QObject
{
    Q_OBJECT

public:
    explicit FolderManager(QObject *parent = nullptr);

    // returns the folder path from the folder created
    // if no folder is created, it returns an empty QString
    Q_INVOKABLE QString createFolder(QString directory, QString folderName) const;

    Q_INVOKABLE bool deleteFolder(QString directory) const;

    // also works with folders
    Q_INVOKABLE bool doesFileExist(QString file) { return QDir(file.remove("file:///")).exists() && !file.isEmpty(); }

    static FolderManager* getInstance();

private:
    static FolderManager* folderManager_Singelton;
};

#endif // FOLDERMANAGER_H
