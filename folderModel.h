#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QtCore>
#include <QtGui>

class FolderModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FolderModel(QObject *parent = nullptr);

    enum LastProjectRoles {
        NameRole = Qt::UserRole + 1,
        SourceRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE bool addNewFolder(QString path);
    Q_INVOKABLE bool removeFolder(QString path);

    Q_INVOKABLE bool openFolder(QString path) const;
    Q_INVOKABLE bool addFromFolderPath(QString path);
private:
    QList<QPair<QString, QString>> m_data{};
    bool addNewFolderToModel(QString path);
protected:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FOLDERMODEL_H
