#include "folderModel.h"
#include "foldermanager.h"

#include <QtGui/QDesktopServices>

FolderModel::FolderModel(QObject *parent)
    : QAbstractListModel(parent) {}

bool FolderModel::addFromFolderPath(QString path) {

    QDirIterator it(path.remove("file:///"));

    bool allFoldersAddedToModel = true;

    while (it.hasNext()) {
        QString subDir = it.next();
        if (!addNewFolderToModel(subDir)) allFoldersAddedToModel = false;
    }
    return allFoldersAddedToModel;
}

bool FolderModel::addNewFolderToModel(QString path) {
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


bool FolderModel::addNewFolder(QString path) {
    return addNewFolderToModel(path.remove("file:///"));
}

bool FolderModel::removeFolder(QString path) {
    for (int i = 0; i < m_data.size(); i++) {
        if (path == m_data[i].second) {
            beginRemoveRows(QModelIndex(), i, i);
            m_data.remove(i);
            endRemoveRows();
            return FolderManager::getInstance()->deleteFolder(path);
        }
    }
    return false;
}

bool FolderModel::openFolder(QString path) const {
    return QDesktopServices::openUrl(path);
}

int FolderModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}


QVariant FolderModel::data(const QModelIndex &index, int role) const {
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

QHash<int, QByteArray> FolderModel::roleNames() const {
    return {
        { NameRole, "name" },
        { SourceRole, "source" }
    };
}

