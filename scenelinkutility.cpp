#include "scenelinkutility.h"
#include "generaltaskutility.h"
#include <QRegularExpression>
#include <QQmlProperty>

SceneLinkUtility* SceneLinkUtility::instance = nullptr;

// unwantedText is the text at the start (like P::) that should be ignored.
// regex is used for splitting and can be ignored when amountOfNumbers 1 is.
// returns an empty vector if an error occurs.


const std::function<bool(QString, QVariantMap&)> SceneLinkUtility::mapN = [](QString line, QVariantMap &map) {
    map["sceneLink"] = line.mid(3);
    return true;
};


const std::function<bool(QString, QVariantMap&)> SceneLinkUtility::mapP = [](QString line, QVariantMap &map) {
    if (auto numbers = GeneralTaskUtility::getNumberValuesFromLine("P::", line, 2, QRegularExpression(";")); !numbers.isEmpty()) {
        map["position"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const std::function<bool(QString, QVariantMap&)> SceneLinkUtility::mapS = [](QString line, QVariantMap &map) {
    if (auto numbers = GeneralTaskUtility::getNumberValuesFromLine("S::", line, 1, QRegularExpression()); !numbers.isEmpty()) {
        map["scale"] = QVariantList(numbers.begin(), numbers.end());
        return true;
    }
    return false;
};

const QMap<QString, std::function<bool(QString, QVariantMap&)>>  SceneLinkUtility::dictionary = {
    {"N::", mapN},
    {"P::", mapP},
    {"S::", mapS}
};

void SceneLinkUtility::generateSceneLink(QQmlComponent* sceneLinkComponent, QQuickItem* scene, QVariantMap &map) {

    QObject* sceneObj = sceneLinkComponent->create();
    QQuickItem* sceneLink = qobject_cast<QQuickItem*>(sceneObj);

    sceneLink->setParentItem(scene);

    QQmlProperty(sceneLink, "x").write(map["position"].toList()[0]);
    QQmlProperty(sceneLink, "y").write(map["position"].toList()[1]);
    QQmlProperty(sceneLink, "sceneLink").write(map["sceneLink"]);

    QVariant s = map["scale"];
    if (s != QVariant()) QQmlProperty(sceneLink, "scale").write(s);
}
