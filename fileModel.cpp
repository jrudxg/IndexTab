#include "fileModel.h"
#include "Filemanager.h"

#include <QDesktopServices>

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent) {}

bool FileModel::addFromFolderPath(QString path) {

    QDirIterator it(path.remove("file:///"));

    bool allFilesAddedToModel = true;

    while (it.hasNext()) {
        QString dir = it.next();
        if (!addNewFileToModel(dir)) allFilesAddedToModel = false;
    }
    return allFilesAddedToModel;
}

bool FileModel::addNewFileToModel(QString path) {
    if (!path.contains('/')) return false;

    for (const auto data : m_data) {
        if (path == data.second) return false;
    }
    QString projectName = path.section('/', -1);
    // skips important data which the user shouldnt have access to
    if (projectName == "." || projectName == "..") return true;

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append({projectName, path});
    endInsertRows();
    return true;
}


bool FileModel::addNewFile(QString path) {
    return addNewFileToModel(path.remove("file:///"));
}

bool FileModel::removeFile(QString path) {
    for (int i = 0; i < m_data.size(); i++) {
        if (path == m_data[i].second) {
            beginRemoveRows(QModelIndex(), i, i);
            m_data.remove(i);
            endRemoveRows();
            return FileManager::getInstance()->deleteFile(path);
        }
    }
    return false;
}

QString FileModel::getTextFromModelEntry(QString fileSource) const {

    fileSource.remove("file:///");
    for (auto data : m_data) {

        if (fileSource == data.second) {

            QFile file(fileSource);
            if (!file.open(QFile::ReadOnly | QFile::Truncate)) return QString();
            QString text = QTextStream(&file).readAll();
            file.close();
            return text;
        }
    }

    return QString();
}
bool FileModel::saveTextToModelEntry(const QString &text, QString fileSource) const {

    fileSource.remove("file:///");
    for (auto data : m_data) {

        if (fileSource == data.second) {

            QFile file(fileSource);
            if (!file.open(QFile::WriteOnly | QFile::Truncate)) return false;
            QTextStream out (&file);
            out << text;
            file.close();
            return true;
        }
    }

    return false;

}

int FileModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}


QVariant FileModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (row < 0 || row > m_data.count()) {
        return QVariant();
    }

    const auto entry = &m_data.at(index.row());

    switch(role) {
        case NameRole:
            return entry->first;
        case SourceRole:
            return entry->second;
    }

    return QVariant();
}

QHash<int, QByteArray> FileModel::roleNames() const {
    return {
        { NameRole, "name" },
        { SourceRole, "source" }
    };
}

