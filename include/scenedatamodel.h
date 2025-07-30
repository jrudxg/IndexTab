#ifndef SCENEDATAMODEL_H
#define SCENEDATAMODEL_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <list>
#include "include/generaltaskutility.h"

class SceneDataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject *currentScene READ currentScene WRITE setCurrentScene NOTIFY
                   currentSceneChanged FINAL)

    Q_PROPERTY(QString currentSceneName READ currentSceneName WRITE setCurrentSceneName NOTIFY
                   currentSceneNameChanged FINAL)

public:
    explicit SceneDataModel(QQmlApplicationEngine &engine, QObject *parent = nullptr);

    // adds an element to a scene and makes the scene its parent
    void addElement(QString sceneName, GeneralTaskUtility::taskTypes type, QVariantMap &map);

    void setCurrentScene(QObject *scene);
    Q_INVOKABLE bool setCurrentScenePerName(const QString newSceneName);

    void setCurrentSceneName(QString name);

    QString currentSceneName();
    QQuickItem *currentScene();

    // a dictionary returning the task type of a given name.
    // Doesn't include any meta types like size or unknown.
    static const QMap<QString, GeneralTaskUtility::taskTypes> taskTypeDictionary;

    static SceneDataModel *getInstance();

    static void resetData() {
        getInstance()->m_data.clear();
        getInstance()->m_currentSceneName.clear();
        getInstance()->sceneHistory.clear();
        getInstance()->m_currentSceneIt = getInstance()->sceneHistory.begin();
    }
    //returns whether currentSceneIt is the last element
    Q_INVOKABLE bool goForwardInSceneHistory();
    //returns whether currentSceneIt is the first element
    Q_INVOKABLE bool goBackInSceneHistory();

private:
    static SceneDataModel *instance;
    // QObject has to be Scene from Scene.qml
    QVector<QPair<QString, QQuickItem*>> m_data;

    QString m_currentSceneName;

    QQmlComponent *m_sceneComponent = nullptr;

    QQmlApplicationEngine *m_engine;

    std::list<QQuickItem*> sceneHistory{};
    std::list<QQuickItem*>::iterator m_currentSceneIt = sceneHistory.begin();

    // can't be out of bounds
    static const QMap<
        GeneralTaskUtility::taskTypes,
        std::function<void(QQmlComponent *component, QQuickItem *scene, QVariantMap &map)>>
        generateTaskDictionary;

    QMap<GeneralTaskUtility::taskTypes, QQmlComponent *> taskComponents;
signals:
    void currentSceneChanged();
    Q_INVOKABLE void currentSceneNameChanged();
};

#endif // SCENEDATAMODEL_H
