#include "tablemodel.h"

TableModel::TableModel(const int rows, const int columns, const QStringList rowHeaders, const QStringList columnHeaders, QObject *parent)
    : QStandardItemModel(rows, columns, parent)
{

    if (rowHeaders == QStringList()) for (int i = 0; i < rows; i++) setHeaderData(i, Qt::Vertical, QString::number(i));
    else for (int i = 0; i < rows; ++i)  {
            if (i < rowHeaders.size()) setHeaderData(i, Qt::Vertical, rowHeaders[i].trimmed());
            else setHeaderData(i, Qt::Vertical, QString::number(i));
    }

    if (columnHeaders == QStringList()) for (int i = 0; i < columns; i++) setHeaderData(i, Qt::Horizontal, QString::number(i));
    else for (int i = 0; i < columns; ++i)  {
            if (i < columnHeaders.size()) setHeaderData(i, Qt::Horizontal, columnHeaders[i].trimmed());
            else setHeaderData(i, Qt::Horizontal, QString::number(i));
    }
}


bool TableModel::addValuesToRow(const QStringList values, const QString verticalHeaderName) {
    if (currentRow >= rowCount() || values.size() > columnCount()) return false;
    if (verticalHeaderName != "") setHeaderData(currentRow, Qt::Vertical, verticalHeaderName.trimmed());

    for (int i = 0; i < values.size(); ++i) setData(index(currentRow, i), values[i].trimmed());

    ++currentRow;

    return true;
}
