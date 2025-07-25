#include "include/filemodel.h"
#include "include/filemanager.h"

#include <QDesktopServices>
#include <QDirIterator>

bool FileModel::addFromFolderPath(QString path)
{
    QDirIterator it(path.remove("file://"));

    bool allFilesAddedToModel = true;

    while (it.hasNext()) {
        QString dir = it.next();
        if (!addNewFileToModel(dir))
            allFilesAddedToModel = false;
    }
    return allFilesAddedToModel;
}

bool FileModel::addNewFileToModel(QString path)
{
    if (!path.contains('/'))
        return false;

    for (auto &data : std::as_const(m_data)) {
        if (path == data.second)
            return false;
    }
    QString projectName = path.section('/', -1);
    // skips important data which the user shouldnt have access to
    if (projectName == "." || projectName == "..")
        return true;

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append({projectName, path});
    endInsertRows();
    return true;
}

bool FileModel::addNewFile(QString path)
{
    return addNewFileToModel(path.remove("file://"));
}

bool FileModel::removeFile(QString path)
{

    for (int i = 0; i < m_data.size(); i++) {
        if (path == m_data[i].second) {
            beginRemoveRows(QModelIndex(), i, i);
            m_data.remove(i);
            endRemoveRows();
            return FileManager::getInstance()->deleteFolder(path);
        }
    }
    return false;
}

QString FileModel::getTextFromModelEntry(QString fileSource) const
{
    // chops of MAIN.txt
    const QString project = fileSource.remove("file://").chopped(9);

    for (auto &data : std::as_const(m_data)) {
        if (project == data.second) {
            QFile file{fileSource};
            if (!file.open(QFile::ReadOnly))
                return QString();
            QString text = QTextStream(&file).readAll();
            return text;
        }
    }

    return QString();
}
bool FileModel::saveTextToModelEntry(const QString &text, QString fileSource) const
{
    // chops of MAIN.txt
    const QString project = fileSource.remove("file:///").chopped(9);
    for (auto &data : std::as_const(m_data)) {
        if (project == data.second) {
            QFile file(fileSource);
            if (!file.open(QFile::WriteOnly | QFile::Truncate))
                return false;
            QTextStream out(&file);
            out << text;
            file.close();
            return true;
        }
    }

    return false;
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (row < 0 || row > m_data.count()) {
        return QVariant();
    }

    const auto entry = &m_data.at(index.row());

    switch (role) {
    case NameRole:
        return entry->first;
    case SourceRole:
        return entry->second;
    }

    return QVariant();
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    return {{NameRole, "name"}, {SourceRole, "source"}};
}
