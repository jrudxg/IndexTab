#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QDir>
#include <QObject>

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr)
        : QObject(parent)
    {}

    // returns the folder path from the folder created
    // if no folder is created, it returns an empty QString
    Q_INVOKABLE QString createFolder(QString directory,
                                     QString fileName,
                                     bool shouldCreateImageFolder = false) const;
    // returns the file path from the file created
    // if no file is created, it returns an empty QString
    Q_INVOKABLE QString createFile(QString directory, QString fileName) const;

    Q_INVOKABLE bool deleteFolder(QString fileDirectory) const;

    Q_INVOKABLE QString getFileUrl();

    static FileManager *getInstance();

private:
    static FileManager *fileManager_Singelton;
};

#endif // FILEMANAGER_H
