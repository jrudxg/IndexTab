#include "projectreader.h"
#include "scenedatamodel.h"
#include "flashcardUtility.h"
#include "scenelinkutility.h"
#include "tableutility.h"
#include <QFile>
#include <QMultiMap>

GeneralTaskUtility::taskTypes getTaskTypeOfString(const QString &line) {

    if (line.size() < 5 || line.mid(3, 2) != "::") return GeneralTaskUtility::taskTypes::unknown;
    QString potentialTaskType = line.first(3);

    if (const auto& it = SceneDataModel::taskTypeDictionary.find(potentialTaskType); it != SceneDataModel::taskTypeDictionary.end()) {
        return it.value();
    }
    return GeneralTaskUtility::taskTypes::unknown;
}



// make sure that there are 3 colons at the end of the string
void ProjectReader::setNewSceneNameFromLine(QString line) {
    currentSceneName = GeneralTaskUtility::removeTrippleColumns(line.trimmed());
}


QString generateObject(GeneralTaskUtility::taskTypes type, QString currentSceneName, QTextStream &in) {

    QVariantMap map{};
    QVector<QString> requiredValuesNeeded;
    QMultiMap<QString, std::function<bool(QString, QVariantMap&)>> dictionary;

    if (type == GeneralTaskUtility::taskTypes::Flashcard) {
        requiredValuesNeeded = FlashcardUtility::getRequiredValuesNeeded();
        dictionary = FlashcardUtility::dictionary;

    }

    if (type == GeneralTaskUtility::taskTypes::Link) {
        requiredValuesNeeded = SceneLinkUtility::getRequiredValuesNeeded();
        dictionary = SceneLinkUtility::dictionary;
    }

    if (type == GeneralTaskUtility::taskTypes::Table) {
        requiredValuesNeeded = TableUtility::getMinimumRequiredValuesNeeded();
        dictionary = TableUtility::dictionary;
        if (in.atEnd()) return "";

        int rowCount = TableUtility::generateTableModel(in.readLine(), map );
        TableUtility::addRowsToValuesAndDictionary(&requiredValuesNeeded, &dictionary, rowCount);
    }


    startOfLoop:
    if (in.atEnd()) dictionary = {};
    QString line = in.readLine();

    // skips when a comment is detected
    if (line.startsWith('#')) goto startOfLoop;

    line = GeneralTaskUtility::removeComment(line);

    for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
        if (!line.startsWith(it.key())) continue;
        if (!dictionary.value(it.key())(line, map)) return "";



        for (int i = 0; i < requiredValuesNeeded.size(); ++i) {
            if (it.key() == requiredValuesNeeded[i]) requiredValuesNeeded.remove(i,1);
        }

        dictionary.erase(it);

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

    GeneralTaskUtility::taskTypes type = GeneralTaskUtility::taskTypes::unknown;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (type == GeneralTaskUtility::taskTypes::unknown) {
            if (line.endsWith(":::")) setNewSceneNameFromLine(line);
            else {
                type = getTaskTypeOfString(line);
            }
            if (type == GeneralTaskUtility::taskTypes::unknown) continue;
        }

        QString lastLine = generateObject(type, currentSceneName, in);
        if (lastLine != "") in.readLineInto(&lastLine);
        type = GeneralTaskUtility::taskTypes::unknown;
    }

    file.close();
}

