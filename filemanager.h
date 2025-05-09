#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QtCore>

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);

    // returns the folder path from the folder created
    // if no folder is created, it returns an empty QString
    Q_INVOKABLE QString createFolder(QString directory, QString fileName) const;
    // returns the file path from the file created
    // if no file is created, it returns an empty QString
    Q_INVOKABLE QString createFile(QString directory, QString fileName) const;

    Q_INVOKABLE bool deleteFile(QString directory) const;

    // also works with Files
    Q_INVOKABLE bool doesFolderExist(QString file) { return QDir(file.remove("file:///")).exists() && !file.isEmpty(); }

    static FileManager* getInstance();

private:
    static FileManager* fileManager_Singelton;
};

#endif // FILEMANAGER_H
