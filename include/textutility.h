#ifndef TEXTUTILITY_H
#define TEXTUTILITY_H

#include <QObject>
#include <QQuickItem>

class TextUtility : public QObject
{
    Q_OBJECT
public:
    explicit TextUtility(QObject *parent = nullptr)
        : QObject{parent} {};

    static const QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> dictionary;

    static const QStringList getEndingSequences() { return {"/TXT", "/MD"}; }

    static const QVector<QString> getRequiredValuesNeeded() { return {"P::"}; }

    static void generateText(QQmlComponent *textComponent, QQuickItem *scene, QVariantMap &map);

    // endingsequences: 0 = /TXT =, 1 = /MD, 2 = /MDE
    static const bool setText(const QString finalText,
                              unsigned int endingSequence,
                              QString imagePath,
                              QVariantMap &map);

private:
    static const std::function<bool(QString, QVariantMap &)> mapP;
    static const std::function<bool(QString, QVariantMap &)> mapS;
};

#endif // TEXTUTILITY_H
