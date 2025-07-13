#include "flashcardUtility.h"
#include "generaltaskutility.h"
#include <QRegularExpression>
#include <QQmlProperty>

FlashcardUtility* FlashcardUtility::instance = nullptr;

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapA = [](QString line, QVariantMap &map) {
    if (!FlashcardUtility::getInstance()->addElementToAnswers(line.mid(3))) return false;
    map["answers"] = line.mid(3);
    return true;
};

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapQ = [](QString line, QVariantMap &map) {
    map["question"] = line.mid(3);
    return true;
};

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapDQ = [](QString line, QVariantMap &map) {
    map["descriptionQuestion"] = line.mid(4);
    return true;
};

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapDA = [](QString line, QVariantMap &map) {
    map["descriptionAnswer"] = line.mid(4);
    return true;
};

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapP = [](QString line, QVariantMap &map) {
    if (auto numbers = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";")); !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap&)> FlashcardUtility::mapS = [](QString line, QVariantMap &map) {
    if (auto number = GeneralTaskUtility::getNumberValuesFromLine("S::", line, 1, QRegularExpression()); !number.isEmpty()) {
        map["scale"] = QVariant(number[0]);
        return true;
    }
    return false;
};




const QMultiMap<QString, std::function<bool(QString, QVariantMap&)>>  FlashcardUtility::dictionary = {
    {"Q::", mapQ},
    {"DQ::", mapDQ},
    {"DA::", mapDA},
    {"P::", mapP},
    {"S::", mapS},
    {"A::", mapA}
};

bool FlashcardUtility::addElementToAnswers(const QString uAnswer) {

    QStringList list = GeneralTaskUtility::splitAtFirstTwoCommas(uAnswer);

    QString cAnswer;

    QStringList unorderedAnswers;
    QStringList orderedAnswers;


    if (list.size() == 1) {
        unorderedAnswers = GeneralTaskUtility::splitAtComma(list[0]);

        for (auto& answer : unorderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        cAnswer = unorderedAnswers.join(", ");
    }
    else if (list.size() == 2) {
        unorderedAnswers = GeneralTaskUtility::splitAtComma(list[0]);
        for (auto& answer : unorderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        orderedAnswers = GeneralTaskUtility::splitAtComma(list[1]);
        for (auto& answer : orderedAnswers) {
            answer = GeneralTaskUtility::removeBackslashBeforeBackslash(GeneralTaskUtility::removeBackslashBeforeComma(answer.trimmed()));
        }

        cAnswer = unorderedAnswers.join(", ") + '\n' + orderedAnswers.join(", ");
    }
    else return false;


    answers.insert(uAnswer, {cAnswer, {unorderedAnswers, orderedAnswers}});
    return true;
}

QString FlashcardUtility::getcAnswerFromuAnswer(QString uAnswer) {
    if (!answers.contains(uAnswer)) return "Error";
    return answers[uAnswer].first;
}

void FlashcardUtility::generateFlashcard(QQmlComponent* flashcardComponent, QQuickItem* scene, QVariantMap &map) {

    QObject* sceneObj = flashcardComponent->create();
    QQuickItem* flashcard = qobject_cast<QQuickItem*>(sceneObj);

    flashcard->setParentItem(scene);


    QQmlProperty(flashcard, "x").write(map["position"].toList()[0]);
    QQmlProperty(flashcard, "y").write(map["position"].toList()[1]);
    QQmlProperty(flashcard, "question").write(map["question"]);
    QQmlProperty(flashcard, "answer").write(map["answers"]);

    QVariant qD = map["descriptionQuestion"];
    if (qD != QVariant()) QQmlProperty(flashcard, "questionDescription").write(qD);

    QVariant aD = map["descriptionAnswer"];
    if (aD != QVariant()) QQmlProperty(flashcard, "answerDescription").write(aD);

    QVariant s = map["scale"];
    if (s != QVariant()) QQmlProperty(flashcard, "scale").write(s);
}
