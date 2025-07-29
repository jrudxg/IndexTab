#include "include/textutility.h"
#include <QQmlProperty>
#include <include/generaltaskutility.h>

const std::function<bool(QString, QVariantMap &)> TextUtility::mapP = [](QString line,
                                                                         QVariantMap &map) {
    if (auto numbers
        = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";"));
        !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap &)> TextUtility::mapS = [](QString line,
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

const QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> TextUtility::dictionary
    = {{"S::", mapS}, {"P::", mapP}};

const bool TextUtility::setText(const QString finalText,
                                unsigned int endingSequence,
                                QString imagePath,
                                QVariantMap &map)
{
    if (endingSequence > getEndingSequences().size())
        return false;
    map["textType"] = endingSequence;

    map["input"] = finalText;

    map["imagePath"] = imagePath;

    return true;
}

void TextUtility::generateText(QQmlComponent *textComponent, QQuickItem *scene, QVariantMap &map)
{
    QObject *sceneObj = textComponent->create();

    QQuickItem *text = qobject_cast<QQuickItem *>(sceneObj);

    text->setParentItem(scene);

    QQmlProperty(text, "positionX").write(map["position"].toList()[0]);
    QQmlProperty(text, "positionY").write(map["position"].toList()[1]);
    QQmlProperty(text, "textType").write(map["textType"]);
    QQmlProperty(text, "input").write(map["input"]);

    QQmlProperty(text, "imagePath").write(map["imagePath"]);

    QVariant scale = map["scale"];
    if (scale != QVariant())
        QQmlProperty(text, "scale").write(scale);
    else {
        QVariantList size = qvariant_cast<QVariantList>(map["size"]);
        if (size != QVariantList()) {
            QQmlProperty(text, "defaultWidth").write(size[0]);
            QQmlProperty(text, "defaultHeight").write(size[1]);
        }
    }
}
