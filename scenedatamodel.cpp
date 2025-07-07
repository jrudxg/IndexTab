#include "scenedatamodel.h"
#include "flashcardUtility.h"
#include "scenelinkutility.h"

SceneDataModel* SceneDataModel::instance = nullptr;

SceneDataModel::SceneDataModel(QQmlApplicationEngine &engine, QObject *parent)
: QObject(parent)
{
    if (SceneDataModel::instance == nullptr) SceneDataModel::instance = this;
    m_engine = &engine;
}


void SceneDataModel::addElement(QString sceneName, taskTypes type, QVariantMap &map) {

    bool nameAlreadyExists = false;
    QQuickItem* neededScene;
    for (auto& scene : m_data) {
        if (sceneName == scene.first) {
            neededScene = scene.second;
            nameAlreadyExists = true;
            break;
        }
    }



    if (m_sceneComponent == nullptr) {
        m_sceneComponent = new QQmlComponent(qobject_cast<QQmlEngine*>(m_engine), QUrl("IndexTab/Scene.qml"));
        taskComponents[taskTypes::Flashcard] = new QQmlComponent(qobject_cast<QQmlEngine*>(m_engine), QUrl("IndexTab/Flashcard.qml"));
        taskComponents[taskTypes::Link] = new QQmlComponent(qobject_cast<QQmlEngine*>(m_engine), QUrl("IndexTab/SceneLink.qml"));
    }

    if (!nameAlreadyExists) {
        QObject* sceneObj = m_sceneComponent->create();
        QQuickItem* scene = qobject_cast<QQuickItem*>(sceneObj);
        m_data.push_back({sceneName, scene});
        if (sceneName == "MAIN") setCurrentScenePerName(sceneName);
        neededScene = scene;
    }

    if (const auto& component = taskComponents.find(type); component != taskComponents.end()) {
        generateTaskDictionary[type](component.value(), neededScene, map);
    }
}


const QMap<taskTypes, std::function<void(QQmlComponent* component, QQuickItem* scene, QVariantMap &map)>> SceneDataModel::generateTaskDictionary = {

    {taskTypes::Flashcard, FlashcardUtility::generateFlashcard},
    {taskTypes::Link, SceneLinkUtility::generateSceneLink},

    // exists so that the map can still be used with meta types and you don't have to check if the type is out of bounds
    {taskTypes::size, [](QQmlComponent* component, QQuickItem* scene, QVariantMap &map){}},
    {taskTypes::unknown, [](QQmlComponent* component, QQuickItem* scene, QVariantMap &map){}}
};

const QMap<QString, taskTypes> SceneDataModel::taskTypeDictionary = {
    {"FLC", taskTypes::Flashcard},
    {"TAB", taskTypes::Table},
    {"LNK", taskTypes::Link}
};

QString SceneDataModel::currentSceneName()
{
    return m_currentSceneName;
}

QQuickItem* SceneDataModel::currentScene() {
    return m_currentScene;
}

SceneDataModel* SceneDataModel::getInstance() {
    return instance;
}

bool SceneDataModel::setCurrentScenePerName(QString newSceneName) {
    if (const auto& it = std::find_if(m_data.begin(), m_data.end(), [&](const auto& scene){return scene.first == newSceneName;}); it != m_data.end()) {
        m_currentScene = it->second;
        emit currentSceneChanged();
        setCurrentSceneName(newSceneName);
        return true;
    }
    for (auto& data : std::as_const(m_data)) {
        if (data.second == m_currentScene) {
            if (data == m_data.last()) {
                m_currentScene = m_data[0].second;
                emit currentSceneChanged();

                setCurrentSceneName(m_data[0].first);
                return false;
            }
            auto* next = &data+1;
            m_currentScene = next->second;
            emit currentSceneChanged();

            setCurrentSceneName(next->first);
            return false;
        }
    }
    return false;
}

void SceneDataModel::setCurrentScene(QObject* scene) {
    m_currentScene = qobject_cast<QQuickItem*>(scene);
    emit currentSceneChanged();
    setCurrentSceneName(std::find_if(m_data.begin(), m_data.end(), [&](const auto& potentialScene){ return potentialScene.second == m_currentScene;})->first);
}

void SceneDataModel::setCurrentSceneName(QString name) {
    m_currentSceneName = name;
    emit currentSceneNameChanged();
}
