#include "include/tablemodel.h"

TableModel::TableModel(const int rows,
                       const int columns,
                       const QStringList rowinclude,
                       const QStringList columninclude,
                       QObject *parent)
    : QStandardItemModel(rows, columns, parent)
{
    if (rowinclude == QStringList())
        for (int i = 0; i < rows; i++)
            setHeaderData(i, Qt::Vertical, QString::number(i+1));
    else
        for (int i = 0; i < rows; ++i) {
            if (i < rowinclude.size())
                setHeaderData(i, Qt::Vertical, rowinclude[i].trimmed());
            else
                setHeaderData(i, Qt::Vertical, QString::number(i+1));
        }

    if (columninclude == QStringList())
        for (int i = 0; i < columns; i++)
            setHeaderData(i, Qt::Horizontal, QString::number(i+1));
    else
        for (int i = 0; i < columns; ++i) {
            if (i < columninclude.size())
                setHeaderData(i, Qt::Horizontal, columninclude[i].trimmed());
            else
                setHeaderData(i, Qt::Horizontal, QString::number(i+1));
        }
}

bool TableModel::addValuesToRow(const QStringList values, const QString verticalHeaderName)
{
    if (currentRow >= rowCount() || values.size() > columnCount())
        return false;
    if (verticalHeaderName != "")
        setHeaderData(currentRow, Qt::Vertical, verticalHeaderName.trimmed());

    for (int i = 0; i < values.size(); ++i)
        setData(index(currentRow, i), values[i].trimmed());

    ++currentRow;

    return true;
}
