#ifndef TABLEUTILITY_H
#define TABLEUTILITY_H

#include <QObject>
#include <QQuickItem>

class TableUtility : public QObject  {
    Q_OBJECT

public:
    explicit TableUtility(QObject* parent = nullptr) : QObject(parent) {}
    static const void addRowsToValuesAndDictionary(QVector<QString>* requiredValues, QMultiMap<QString, std::function<bool(QString, QVariantMap&)>>* dictionary, const int rowCount);
    static const int generateTableModel(QString line, QVariantMap &map);
    static void generateTable(QQmlComponent* tableComponent, QQuickItem* scene, QVariantMap &map);

    static const QVector<QString> getMinimumRequiredValuesNeeded() { return {"P::"}; }

    static const QMultiMap<QString, std::function<bool(QString, QVariantMap&)>> dictionary;

    struct tableSize {
        int rowCount;
        int columnCount;
    };

private:
    static const std::function<bool(QString, QVariantMap&)> mapP;
    static const std::function<bool(QString, QVariantMap&)> mapS;
    static const std::function<bool(QString, QVariantMap&)> mapH;
    static const std::function<bool(QString, QVariantMap&)> mapR;
};















#endif // TABLEUTILITY_H
