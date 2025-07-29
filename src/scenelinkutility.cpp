#include "include/scenelinkutility.h"
#include <QQmlProperty>
#include <QRegularExpression>
#include "include/generaltaskutility.h"

// unwantedText is the text at the start (like P::) that should be ignored.
// regex is used for splitting and can be ignored when amountOfNumbers 1 is.
// returns an empty vector if an error occurs.

const std::function<bool(QString, QVariantMap &)> SceneLinkUtility::mapN = [](QString line,
                                                                              QVariantMap &map) {
    map["sceneLink"] = line.mid(3).trimmed();
    return true;
};

const std::function<bool(QString, QVariantMap &)> SceneLinkUtility::mapP = [](QString line,
                                                                              QVariantMap &map) {
    if (auto numbers
        = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";"));
        !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap &)> SceneLinkUtility::mapS = [](QString line,
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

const QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> SceneLinkUtility::dictionary
    = {{"N::", mapN}, {"P::", mapP}, {"S::", mapS}};

void SceneLinkUtility::generateSceneLink(QQmlComponent *sceneLinkComponent,
                                         QQuickItem *scene,
                                         QVariantMap &map)
{
    QObject *sceneObj = sceneLinkComponent->create();
    QQuickItem *sceneLink = qobject_cast<QQuickItem *>(sceneObj);

    sceneLink->setParentItem(scene);

    QQmlProperty(sceneLink, "positionX").write(map["position"].toList()[0]);
    QQmlProperty(sceneLink, "positionY").write(map["position"].toList()[1]);
    QQmlProperty(sceneLink, "sceneLink").write(map["sceneLink"]);

    QVariant scale = map["scale"];
    if (scale != QVariant())
        QQmlProperty(sceneLink, "scale").write(scale);
    else {
        QVariantList size = qvariant_cast<QVariantList>(map["size"]);
        if (size != QVariantList()) {
            QQmlProperty(sceneLink, "defaultWidth").write(size[0]);
            QQmlProperty(sceneLink, "defaultHeight").write(size[1]);
        }
    }
}
