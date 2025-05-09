#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QtCore>

class FileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FileModel(QObject *parent = nullptr);

    enum LastProjectRoles {
        NameRole = Qt::UserRole + 1,
        SourceRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE bool addNewFile(QString path);
    Q_INVOKABLE bool removeFile(QString path);

    // returns an empty string if no TextStream could be created
    Q_INVOKABLE QString getTextFromModelEntry(QString fileSource) const ;
    Q_INVOKABLE bool saveTextToModelEntry(const QString &text, QString fileSource) const;

    Q_INVOKABLE bool addFromFolderPath(QString path);
private:
    QList<QPair<QString, QString>> m_data{};
    bool addNewFileToModel(QString path);
protected:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FILEMODEL_H
