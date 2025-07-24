#ifndef GENERALTASKUTILITY_H
#define GENERALTASKUTILITY_H

#include <QObject>
#include <QRegularExpression>
#include <QFile>

class GeneralTaskUtility : QObject
{
    Q_OBJECT
public:
    explicit GeneralTaskUtility(QObject* parent = nullptr)
        : QObject{parent} {}
    static const QVector<float> getNumberValuesFromLine(const QString &unwantedText, const QString &line, const int amountOfNumbers, const QRegularExpression regex);

    static const QStringList splitAtFirstTwoCommas(const QString &line);

    static const QStringList splitAtComma(const QString &line);

    static const QString removeBackslashBeforeComma(const QString &line);

    static const QString removeBackslashBeforeBackslash(const QString &line);

    static const QString removeComment(const QString &line);

    static const QString removeTrippleColumns(const QString &line);

    enum taskTypes {
        Flashcard,
        Table,
        Link,
        Text,
        size,
        unknown
    };
};

#endif // GENERALTASKUTILITY_H
