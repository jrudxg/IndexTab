#include "include/flashcardUtility.h"
#include <QQmlProperty>
#include <QRegularExpression>
#include "include/generaltaskutility.h"

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapA = [](QString line,
                                                                              QVariantMap &map) {
    map["answers"] = getcAnswerFromuAnswer(line.mid(3));
    return true;
};

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapQ = [](QString line,
                                                                              QVariantMap &map) {
    map["question"] = line.mid(3);
    return true;
};

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapDQ = [](QString line,
                                                                               QVariantMap &map) {
    map["descriptionQuestion"] = line.mid(4);
    return true;
};

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapDA = [](QString line,
                                                                               QVariantMap &map) {
    map["descriptionAnswer"] = line.mid(4);
    return true;
};

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapP = [](QString line,
                                                                              QVariantMap &map) {
    if (auto numbers
        = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";"));
        !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap &)> FlashcardUtility::mapS = [](QString line,
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

const QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> FlashcardUtility::dictionary
    = {{"Q::", mapQ}, {"DQ::", mapDQ}, {"DA::", mapDA}, {"P::", mapP}, {"S::", mapS}, {"A::", mapA}};

const QString FlashcardUtility::getcAnswerFromuAnswer(const QString uAnswer)
{
    QStringList list = GeneralTaskUtility::splitAtFirstTwoCommas(uAnswer);

    QString cAnswer;

    QStringList unorderedAnswers;
    QStringList orderedAnswers;

    if (list.size() == 1) {
        unorderedAnswers = GeneralTaskUtility::splitAtComma(list[0]);

        for (auto &answer : unorderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(
                GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        cAnswer = unorderedAnswers.join(", ");
    } else if (list.size() == 2) {
        unorderedAnswers = GeneralTaskUtility::splitAtComma(list[0]);
        for (auto &answer : unorderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(
                GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        orderedAnswers = GeneralTaskUtility::splitAtComma(list[1]);
        for (auto &answer : orderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(
                GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        cAnswer = unorderedAnswers.join(", ") + '\n' + orderedAnswers.join(", ");
    }
    return cAnswer;
}

void FlashcardUtility::generateFlashcard(QQmlComponent *flashcardComponent,
                                         QQuickItem *scene,
                                         QVariantMap &map)
{
    QObject *sceneObj = flashcardComponent->create();
    QQuickItem *flashcard = qobject_cast<QQuickItem *>(sceneObj);

    flashcard->setParentItem(scene);

    QQmlProperty(flashcard, "defaultX").write(map["position"].toList()[0]);
    QQmlProperty(flashcard, "defaultY").write(map["position"].toList()[1]);
    QQmlProperty(flashcard, "question").write(map["question"]);
    QQmlProperty(flashcard, "answer").write(map["answers"]);

    QVariant dQ = map["descriptionQuestion"];
    if (dQ != QVariant())
        QQmlProperty(flashcard, "questionDescription").write(dQ);

    QVariant dA = map["descriptionAnswer"];
    if (dA != QVariant())
        QQmlProperty(flashcard, "answerDescription").write(dA);

    QVariant scale = map["scale"];
    if (scale != QVariant())
        QQmlProperty(flashcard, "scale").write(scale);
    else {
        QVariantList size = qvariant_cast<QVariantList>(map["size"]);
        if (size != QVariantList()) {
            QQmlProperty(flashcard, "defaultWidth").write(size[0]);
            QQmlProperty(flashcard, "defaultHeight").write(size[1]);
        }
    }
}
