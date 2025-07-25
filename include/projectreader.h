#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include <QDir>
#include <QObject>
#include "generaltaskutility.h"

extern "C" {
#include "quickjs.h"
}

class ProjectReader : public QObject
{
    Q_OBJECT

public:
    explicit ProjectReader(QObject *parent = nullptr)
        : QObject{parent}
    {}

    void readDir(QString path);

private:
    QString generateObject(GeneralTaskUtility::taskTypes type,
                           QString currentSceneName,
                           QTextStream &in);

    QString generateHTMLFromMarkdown(QString markdown);

    QString insertHTMLinHTMLTemplate(QString textToInsert);

    QString HTMLTemplateText = "";

    QString imagePath{};

    QString currentSceneName;
    QDir *currentDir;

    void setNewSceneNameFromLine(QString line);

    JSRuntime *rt = nullptr;
    JSContext *ctx = nullptr;
};

#endif // PROJECTREADER_H
