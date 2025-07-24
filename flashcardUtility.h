#ifndef FLASHCARDUTILITY_H
#define FLASHCARDUTILITY_H

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariantMap>

/*
 * uAnswer (unchanged answer) is used when descrbing the normal text, the user writes.
 * A,,B,,,C,,D would be an example of it.
 * Generally the user shouldn't see it, as it looks not very good
 *
 * cAnswer (changed answer) is used when descibing the answer, flashcardUtility created from uAnswer.
 * A; B; C; D would be an example of it.
 * Generally the user should see that instead of uAnswer
*/

class FlashcardUtility : public QObject
{

    Q_OBJECT

public:
    explicit FlashcardUtility(QObject* parent = nullptr)
        : QObject{parent} {}
    // creates unordered and ordered answers and adds them with answer as the key to the map.
    // uAnswer (unchanged answer) is used when descrbing the normal text, the user writes.
    //// returns false if no element could be created
    //bool addElementToAnswers(const QString uAnswer);

    // cAnswer (changed answer) is used when descibing the answer, flashcardUtility created from uAnswer.
    // uAnswer (unchanged answer) is used when descrbing the normal text, the user writes.
    static const QString getcAnswerFromuAnswer(const QString uAnswer);

    static void generateFlashcard(QQmlComponent* flashcardComponent, QQuickItem* scene, QVariantMap &map);

    static const QMultiMap<QString, std::function<bool(QString, QVariantMap&)>> dictionary;

    static const QVector<QString> getRequiredValuesNeeded() { return {"Q::","A::","P::"}; }


private:

    static const std::function<bool(QString, QVariantMap&)> mapA;

    static const std::function<bool(QString, QVariantMap&)> mapQ;

    static const std::function<bool(QString, QVariantMap&)> mapDQ;

    static const std::function<bool(QString, QVariantMap&)> mapDA;

    static const std::function<bool(QString, QVariantMap&)> mapP;

    static const std::function<bool(QString, QVariantMap&)> mapS;
};
#endif // FLASHCARDUTILITY_H
