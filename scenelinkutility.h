#ifndef SCENELINKUTILITY_H
#define SCENELINKUTILITY_H

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariantMap>


class SceneLinkUtility : public QObject
{
    Q_OBJECT

public:
    explicit SceneLinkUtility(QObject* parent = nullptr)
    : QObject{parent}
    { if (SceneLinkUtility::instance == nullptr) SceneLinkUtility::instance = this; }

    static void generateSceneLink(QQmlComponent* sceneLinkComponent, QQuickItem* scene, QVariantMap &map);

    static const QMap<QString, std::function<bool(QString, QVariantMap&)>> dictionary;

    static const QVector<QString> getRequiredValuesNeeded() { return {"N::","P::"}; }
private:
    static SceneLinkUtility* instance;

    static const std::function<bool(QString, QVariantMap&)> mapN;


    static const std::function<bool(QString, QVariantMap&)> mapP;

    static const std::function<bool(QString, QVariantMap&)> mapS;
};

#endif // SCENELINKUTILITY_H
