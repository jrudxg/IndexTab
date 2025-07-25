#include "include/generaltaskutility.h"

const QVector<float> GeneralTaskUtility::getNumberValuesFromLine(const QString &unwantedText,
                                                                 const QString &line,
                                                                 const int amountOfNumbers,
                                                                 const QRegularExpression regex)
{
    QString potentialNumbers = line.mid(unwantedText.size());
    QStringList potentialNumberList;
    if (amountOfNumbers > 1) {
        potentialNumberList = potentialNumbers.split(regex);

    } else {
        potentialNumberList += potentialNumbers;
    }
    if (potentialNumberList.size() != amountOfNumbers)
        return QVector<float>();

    QVector<float> numbers;
    for (auto &potentialNumber : potentialNumberList) {
        bool ok = false;
        numbers.push_back(potentialNumber.trimmed().toFloat(&ok));
        if (!ok)
            return QVector<float>();
    }

    return numbers;
}

const QStringList GeneralTaskUtility::splitAtFirstTwoCommas(const QString &line)
{
    int backslashCounter = 0;
    for (int i = 0; i < line.size() - 1; ++i) {
        if (line[i] == '\\')
            ++backslashCounter;
        else if (line[i] == ',' && line[i + 1] == ',' && backslashCounter % 2 == 0) {
            return QStringList{{line.left(i), line.right(line.size() - (i + 2))}};
        } else
            backslashCounter = 0;
    }
    return QStringList{line};
}

const QStringList GeneralTaskUtility::splitAtComma(const QString &line)
{
    QStringList list;
    int lastCommaPosition = 0;
    int backslashCounter = 0;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '\\')
            ++backslashCounter;
        else if (line[i] == ',' && backslashCounter % 2 == 0) {
            if (i >= 2 && line[i] == ',' && line[i - 1] == ',' && line[i - 2] == '\\')
                continue;
            list << line.mid(lastCommaPosition, i - lastCommaPosition);
            lastCommaPosition = i + 1;
            backslashCounter = 0;
        } else
            backslashCounter = 0;
    }
    return list << line.right(line.size() - (lastCommaPosition));
}

const QString GeneralTaskUtility::removeBackslashBeforeComma(const QString &line)
{
    QString string = line;
    for (int i = 0; i < string.size() - 1; ++i) {
        if (string[i] == '\\' && string[i + 1] == ',')
            string.remove(i, 1);
    }
    return string;
}

const QString GeneralTaskUtility::removeBackslashBeforeBackslash(const QString &line)
{
    QString string = line;
    for (int i = 0; i < string.size() - 1; ++i) {
        if (string[i] == '\\' && string[i + 1] == '\\')
            string.remove(i, 1);
    }
    return string;
}

const QString GeneralTaskUtility::removeComment(const QString &line)
{
    QString string = line;
    int backslashCounter = 0;

    for (int i = 0; i < string.size(); ++i) {
        if (string[i] == '\\')
            ++backslashCounter;
        else if (string[i] == '#') {
            if (backslashCounter % 2 == 0)
                return string.remove(i, string.size() - i);
            else
                string.remove(i - 1, 1);
            backslashCounter = 0;
        } else
            backslashCounter = 0;
    }
    return string;
}

const QString GeneralTaskUtility::removeTrippleColumns(const QString &line)
{
    QString string = line;
    return string.remove(":::");
}
