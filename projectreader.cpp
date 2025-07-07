#include "projectreader.h"
#include "scenedatamodel.h"
#include "flashcardUtility.h"
#include "scenelinkutility.h"
#include <QRegularExpression>
#include <QFile>

const QRegularExpression ProjectReader::removeTrippleColumns(":{3}$|\\s");

taskTypes getTaskTypeOfString(const QString &line) {

    if (line.size() < 5 || line.mid(3, 2) != "::") return taskTypes::unknown;
    QString potentialTaskType = line.first(3);

    if (const auto& it = SceneDataModel::taskTypeDictionary.find(potentialTaskType); it != SceneDataModel::taskTypeDictionary.end()) {
        return it.value();
    }
    return taskTypes::unknown;
}



// make sure that there are 3 colons at the end of the string
void ProjectReader::setNewSceneNameFromLine(QString line) {
    currentSceneName = line.trimmed().remove(removeTrippleColumns);
}


QString generateObject(taskTypes type, QString currentSceneName, QTextStream &in) {

    QVariantMap map{};
    QVector<QString> requiredValuesNeeded;
    QMap<QString, std::function<bool(QString, QVariantMap&)>> dictionary;

    if (type == taskTypes::Flashcard) {

        requiredValuesNeeded = FlashcardUtility::getRequiredValuesNeeded();
        dictionary = FlashcardUtility::dictionary;
    }

    if (type == taskTypes::Link) {
        requiredValuesNeeded = SceneLinkUtility::getRequiredValuesNeeded();
        dictionary = SceneLinkUtility::dictionary;
    }

    startOfLoop:
    QString line = in.readLine();

    // skips when a comment is detected
    if (line.startsWith('#')) goto startOfLoop;
    for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
        if (!line.startsWith(it.key())) continue;
        if (!dictionary[it.key()](line, map)) return "";
        requiredValuesNeeded.erase(
            std::remove_if(
                requiredValuesNeeded.begin(),
                requiredValuesNeeded.end(),
                [&](const QString& s) {
                    return s == it.key();
                }
            ),
            requiredValuesNeeded.end()
        );
        dictionary.remove(it.key());

        goto startOfLoop;
    }

    if (requiredValuesNeeded.size() == 0) {
        SceneDataModel::getInstance()->addElement(currentSceneName, type, map);
    }

    return dictionary.size() == 0 ? "" : line;
}


void ProjectReader::readFile(QString source) {
    QFile file(source.remove("file:///"));
    if (!file.open(QFile::ReadOnly)) return;

    taskTypes type = taskTypes::unknown;

    QTextStream in(&file);

    while (!in.atEnd()) {
        in.skipWhiteSpace();
        QString line = in.readLine();

        startOfCheck:
        if (line.startsWith('#')) continue;

        if (type == taskTypes::unknown) {
            type = getTaskTypeOfString(line);
            if (type == taskTypes::unknown && line.endsWith(":::")) setNewSceneNameFromLine(line);
        }

        if (type == taskTypes::Table) {
            type = taskTypes::unknown;
            continue;
        }

        QString lastLine = generateObject(type, currentSceneName, in);
        if (lastLine == "") {
            type = taskTypes::unknown;
            continue;
        }
        type = unknown;
        line = lastLine;
        goto startOfCheck;

    }

    file.close();
}

