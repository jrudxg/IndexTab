#include "include/tableutility.h"
#include <QQmlProperty>
#include "include/generaltaskutility.h"
#include "include/tablemodel.h"

const std::function<bool(QString, QVariantMap &)> TableUtility::mapP = [](QString line,
                                                                          QVariantMap &map) {
    if (auto numbers
        = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";"));
        !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap &)> TableUtility::mapS = [](QString line,
                                                                          QVariantMap &map) {
    if (auto number
        = GeneralTaskUtility::getNumberValuesFromLine("S::", line, 1, QRegularExpression());
        !number.isEmpty()) {
        map["scale"] = QVariant(number[0]);
        return true;
    }
    if (auto numbers
        = GeneralTaskUtility::getNumberValuesFromLine("S::", line, 2, QRegularExpression(";"));
        !numbers.isEmpty()) {
        map["size"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap &)> TableUtility::mapH = [](QString line,
                                                                          QVariantMap &map) {
    map["header"] = line.mid(3);
    return true;
};

const std::function<bool(QString, QVariantMap &)> TableUtility::mapR = [](QString line,
                                                                          QVariantMap &map) {
    TableModel *model = map["model"].value<TableModel *>();
    if (model == nullptr) {
        return false;
    }
    QString verticalHeaderName = line.mid(3).section(":", 0, 0);
    QString unsplittedValues = line.mid(3).section(":", 1);
    if (unsplittedValues.isEmpty()) {
        // verticalHeaderName does not exist
        // and holds the data which is supposed to be for unsplittedValues
        unsplittedValues = verticalHeaderName;
        verticalHeaderName = "";
    }

    QStringList values = unsplittedValues.split(",");
    return model->addValuesToRow(values, verticalHeaderName);
};

const QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> TableUtility::dictionary
    = {{"H::", mapH}, {"S::", mapS}, {"P::", mapP}};

const void TableUtility::addRowsToValuesAndDictionary(
    QVector<QString> *requiredValues,
    QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> *dictionary,
    const int rowCount)
{
    requiredValues->reserve(requiredValues->size() + rowCount);
    for (int i = 0; i < rowCount; ++i) {
        requiredValues->push_back("R::");
        dictionary->insert("R::", mapR);
    }
}

void TableUtility::generateTable(QQmlComponent *tableComponent, QQuickItem *scene, QVariantMap &map)
{
    QObject *sceneObj = tableComponent->create();

    QQuickItem *table = qobject_cast<QQuickItem *>(sceneObj);

    table->setParentItem(scene);

    QQmlProperty(table, "positionX").write(map["position"].toList()[0]);
    QQmlProperty(table, "positionY").write(map["position"].toList()[1]);

    QQmlProperty(table, "model").write(map["model"]);

    QVariant h = map["header"];
    if (h != QVariant())
        QQmlProperty(table, "header").write(h);

    QVariant scale = map["scale"];
    if (scale != QVariant())
        QQmlProperty(table, "scale").write(scale);
    else {
        QVariantList size = qvariant_cast<QVariantList>(map["size"]);
        if (size != QVariantList()) {
            QQmlProperty(table, "defaultWidth").write(size[0]);
            QQmlProperty(table, "defaultHeight").write(size[1]);
        }
    }
}

const int TableUtility::generateTableModel(QString line, QVariantMap &map)
{
    QStringList values = GeneralTaskUtility::splitAtComma(line);
    QStringList potentialSize = values[0].split(';');

    if (potentialSize.size() != 2)
        return 0;

    tableSize size;
    {
        bool Ok1, Ok2;
        size.rowCount = potentialSize[0].toInt(&Ok1);
        size.columnCount = potentialSize[1].toInt(&Ok2);
        if (Ok1 == false || Ok2 == false) {
            return 0;
        }
    }

    QStringList columninclude{};
    for (int i = 1; i < values.size(); ++i) {
        columninclude << values[i];
    }

    if (columninclude.size() > size.columnCount)
        return 0;
    TableModel *model = new TableModel(size.rowCount,
                                       size.columnCount,
                                       QStringList(),
                                       columninclude);
    map["model"] = QVariant::fromValue(model);
    return size.rowCount;
}
