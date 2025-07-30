#include "include/scenedatamodel.h"
#include "include/flashcardUtility.h"
#include "include/scenelinkutility.h"
#include "include/tableutility.h"
#include "include/textutility.h"

SceneDataModel *SceneDataModel::instance = nullptr;

SceneDataModel::SceneDataModel(QQmlApplicationEngine &engine, QObject *parent)
    : QObject(parent)
{
    if (SceneDataModel::instance == nullptr)
        SceneDataModel::instance = this;
    m_engine = &engine;
}

void SceneDataModel::addElement(QString sceneName,
                                GeneralTaskUtility::taskTypes type,
                                QVariantMap &map)
{

    bool nameAlreadyExists = false;
    QQuickItem *neededScene;

    QString adjustedSceneName = sceneName.trimmed().toLower();
    for (auto &scene : m_data) {
        if (adjustedSceneName == scene.first) {
            neededScene = scene.second;
            nameAlreadyExists = true;
            break;
        }
    }

    if (m_sceneComponent == nullptr) {
        QQmlEngine *qmlEngine = qobject_cast<QQmlEngine *>(m_engine);
        m_sceneComponent = new QQmlComponent(qmlEngine, QUrl("IndexTab/Scene.qml"));
        taskComponents[GeneralTaskUtility::taskTypes::Flashcard] = new QQmlComponent(qmlEngine, QUrl("IndexTab/TaskFlashcard.qml"));
        taskComponents[GeneralTaskUtility::taskTypes::Link] = new QQmlComponent(qmlEngine, QUrl("IndexTab/TaskSceneLink.qml"));
        taskComponents[GeneralTaskUtility::taskTypes::Table] = new QQmlComponent(qmlEngine, QUrl("IndexTab/TaskTable.qml"));
        taskComponents[GeneralTaskUtility::taskTypes::Text] = new QQmlComponent(qmlEngine, QUrl("IndexTab/TaskText.qml"));
    }

    if (!nameAlreadyExists) {
        QObject *sceneObj = m_sceneComponent->create();
        QQuickItem *scene = qobject_cast<QQuickItem *>(sceneObj);
        m_data.push_back({adjustedSceneName, scene});
        if (adjustedSceneName == "main") {
            setCurrentScenePerName(adjustedSceneName);
        }
        neededScene = scene;
    }
    if (const auto &component = taskComponents.find(type); component != taskComponents.end()) {
        generateTaskDictionary[type](component.value(), neededScene, map);
    }
}

const QMap<GeneralTaskUtility::taskTypes,
           std::function<void(QQmlComponent *component, QQuickItem *scene, QVariantMap &map)>>
    SceneDataModel::generateTaskDictionary = {

        {GeneralTaskUtility::taskTypes::Flashcard, FlashcardUtility::generateFlashcard},
        {GeneralTaskUtility::taskTypes::Link, SceneLinkUtility::generateSceneLink},
        {GeneralTaskUtility::taskTypes::Table, TableUtility::generateTable},
        {GeneralTaskUtility::taskTypes::Text, TextUtility::generateText},

        // exists so that the map can still be used with meta types and you don't have to check if the type is out of bounds
        {GeneralTaskUtility::taskTypes::size,
         [](QQmlComponent *component, QQuickItem *scene, QVariantMap &map) {}},
        {GeneralTaskUtility::taskTypes::unknown,
         [](QQmlComponent *component, QQuickItem *scene, QVariantMap &map) {}}};

const QMap<QString, GeneralTaskUtility::taskTypes> SceneDataModel::taskTypeDictionary
    = {{"FLC", GeneralTaskUtility::taskTypes::Flashcard},
       {"TAB", GeneralTaskUtility::taskTypes::Table},
       {"LNK", GeneralTaskUtility::taskTypes::Link},
       {"TXT", GeneralTaskUtility::taskTypes::Text}};

QString SceneDataModel::currentSceneName()
{
    return m_currentSceneName;
}

QQuickItem *SceneDataModel::currentScene()
{
    return *m_currentSceneIt;
}

SceneDataModel *SceneDataModel::getInstance()
{
    return instance;
}

bool SceneDataModel::setCurrentScenePerName(QString newSceneName)
{
    QString adjustedSceneName = newSceneName.trimmed().toLower();
    if (const auto &it = std::find_if(m_data.begin(),
                                      m_data.end(),
                                      [&](const auto &scene) {
                                          return scene.first == adjustedSceneName;
                                      });

    it != m_data.end()) {
        if (std::next(m_currentSceneIt) != sceneHistory.end()) sceneHistory.erase(std::next(m_currentSceneIt), sceneHistory.end());
        sceneHistory.push_back(it->second);
        m_currentSceneIt = std::prev(sceneHistory.end());
        emit currentSceneChanged();
        setCurrentSceneName(adjustedSceneName);

        return true;
    }
    return false;
}

void SceneDataModel::setCurrentScene(QObject *scene)
{
    if (std::next(m_currentSceneIt) != sceneHistory.end()) sceneHistory.erase(std::next(m_currentSceneIt), sceneHistory.end());
    sceneHistory.push_back(qobject_cast<QQuickItem*>(scene));
    m_currentSceneIt = std::prev(sceneHistory.end());
    emit currentSceneChanged();

    setCurrentSceneName(std::find_if(m_data.begin(), m_data.end(), [&](const auto &potentialScene) {
                            return potentialScene.second == *m_currentSceneIt;
                        })->first);
}

void SceneDataModel::setCurrentSceneName(QString name)
{
    m_currentSceneName = name.trimmed();
    emit currentSceneNameChanged();
}

bool SceneDataModel::goBackInSceneHistory() {
    if (m_currentSceneIt == sceneHistory.begin()) return true;
    m_currentSceneIt = std::prev(m_currentSceneIt);
    emit currentSceneChanged();

    setCurrentSceneName(std::find_if(m_data.begin(), m_data.end(), [&](const auto &potentialScene) {
                            return potentialScene.second == *m_currentSceneIt;
                        })->first);
    return std::prev(m_currentSceneIt) == sceneHistory.begin();
}

bool SceneDataModel::goForwardInSceneHistory() {
    if (auto nextIt = std::next(m_currentSceneIt); nextIt != sceneHistory.end()) {
        m_currentSceneIt = nextIt;
        emit currentSceneChanged();

        setCurrentSceneName(std::find_if(m_data.begin(), m_data.end(), [&](const auto &potentialScene) {
                                return potentialScene.second == *m_currentSceneIt;
                            })->first);
        return std::next(m_currentSceneIt) == sceneHistory.end();
    }
    return true;
}
