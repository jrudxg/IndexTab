#include "generaltaskutility.h"

const QVector<float> GeneralTaskUtility::getNumberValuesFromLine(const QString &unwantedText, const QString &line, const int amountOfNumbers, const QRegularExpression regex) {
    QString potentialNumbers = line.mid(unwantedText.size());
    QStringList potentialNumberList;
    if (amountOfNumbers > 1) {
        potentialNumberList = potentialNumbers.split(regex);

    }
    else {
        potentialNumberList += potentialNumbers;
    }
    if (potentialNumberList.size() != amountOfNumbers) return QVector<float>();

    QVector<float> numbers;
    for (auto &potentialNumber : potentialNumberList) {
        bool ok = false;
        numbers.push_back(potentialNumber.trimmed().toFloat(&ok));
        if (!ok) return QVector<float>();
    }

    return numbers;
}

