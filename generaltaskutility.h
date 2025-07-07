#ifndef GENERALTASKUTILITY_H
#define GENERALTASKUTILITY_H

#include <QObject>
#include <QRegularExpression>

class GeneralTaskUtility : QObject
{
    Q_OBJECT
public:
    explicit GeneralTaskUtility(QObject* parent = nullptr)
        : QObject{parent} {}
    static const QVector<float> getNumberValuesFromLine(const QString &unwantedText, const QString &line, const int amountOfNumbers, const QRegularExpression regex);
};

#endif // GENERALTASKUTILITY_H
