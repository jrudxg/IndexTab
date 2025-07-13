#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QStandardItemModel>

class TableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TableModel(const int rows, const int columns, const QStringList rowHeaders = QStringList(), const QStringList columnHeaders = QStringList(), QObject *parent = nullptr);

    bool addValuesToRow(const QStringList values, const QString verticalHeaderName = "");

private:
    int currentRow{};

signals:
};

#endif // TABLEMODEL_H

Q_DECLARE_METATYPE(TableModel*);
