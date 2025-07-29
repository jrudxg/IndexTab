#include "include/projectreader.h"
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>
#include <QMultiMap>
#include "include/flashcardUtility.h"
#include "include/scenedatamodel.h"
#include "include/scenelinkutility.h"
#include "include/tableutility.h"
#include "include/textutility.h"


/*
The library quickjs (https://github.com/quickjs-ng/quickjs) included in this product is licensed under:

The MIT License (MIT)

Copyright (c) 2017-2024 Fabrice Bellard
Copyright (c) 2017-2024 Charlie Gordon
Copyright (c) 2023-2025 Ben Noordhuis
Copyright (c) 2023-2025 Saúl Ibarra Corretgé

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

// generates with markdown-it and many plugins a html file
// uses quickjs (https://github.com/quickjs-ng/quickjs)
QString ProjectReader::generateHTMLFromMarkdown(QString markdown)
{
    JSValue global = JS_GetGlobalObject(ctx);
    QString html = "";

    markdown.replace("\\{", "\\\\{");
    markdown.replace("\\}", "\\\\}");

    JSValue object = JS_GetPropertyStr(ctx, global, "convertMarkdownToHTML");
    JSValue func = JS_GetPropertyStr(ctx, object, "convertMarkdownToHTML");

    JSValue arg[1];
    arg[0] = JS_NewString(ctx, markdown.toStdString().c_str());

    JSValue result = JS_Call(ctx, func, object, 1, arg);

    const char *cstr = JS_ToCString(ctx, result);

    html = QString::fromUtf8(cstr);

    JS_FreeCString(ctx, cstr);

    JS_FreeValue(ctx, result);
    JS_FreeValue(ctx, arg[0]);

    JS_FreeValue(ctx, func);
    JS_FreeValue(ctx, object);
    JS_FreeValue(ctx, global);

    return html;
}

QString ProjectReader::insertHTMLinHTMLTemplate(QString textToInsert)
{
    QString copyOfHTMLTexmplateText = HTMLTemplateText;
    return copyOfHTMLTexmplateText.replace("${result}", textToInsert);
}

GeneralTaskUtility::taskTypes getTaskTypeOfString(const QString &line)
{
    if (line.size() < 5 || line.mid(3, 2) != "::")
        return GeneralTaskUtility::taskTypes::unknown;
    QString potentialTaskType = line.first(3);

    if (const auto &it = SceneDataModel::taskTypeDictionary.find(potentialTaskType);
        it != SceneDataModel::taskTypeDictionary.end()) {
        return it.value();
    }
    return GeneralTaskUtility::taskTypes::unknown;
}

// make sure that there are 3 colons at the end of the string
void ProjectReader::setNewSceneNameFromLine(QString line)
{
    currentSceneName = GeneralTaskUtility::removeTrippleColumns(line.trimmed());
}

QString ProjectReader::generateObject(GeneralTaskUtility::taskTypes type,
                                      QString currentSceneName,
                                      QTextStream &in)
{
    QVariantMap map{};
    QVector<QString> requiredValuesNeeded;
    QMultiMap<QString, std::function<bool(QString, QVariantMap &)>> dictionary;

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

        int rowCount = TableUtility::generateTableModel(in.readLine(), map);
        TableUtility::addRowsToValuesAndDictionary(&requiredValuesNeeded, &dictionary, rowCount);
    }

    if (type == GeneralTaskUtility::taskTypes::Text) {
        requiredValuesNeeded = TextUtility::getRequiredValuesNeeded();
        dictionary = TextUtility::dictionary;

        QString text{};

        QStringList endingSequences = TextUtility::getEndingSequences();
        QString line = in.readLine();

        for (; !endingSequences.contains(line); line = in.readLine()) {
            if (in.atEnd())
                return "";
            text.append(line + '\n');
        }

        int textType = endingSequences.indexOf(line);
        if (textType == -1) {
            return "";
        }
        if (line == "/MD") {
            QString textToBeInserted = generateHTMLFromMarkdown(text);
            text = insertHTMLinHTMLTemplate(textToBeInserted);
        }
        TextUtility::setText(text, textType, imagePath, map);
    }

startOfLoop:
    if (in.atEnd())
        dictionary = {};
    QString line = in.readLine();

    // skips when a comment is detected
    if (line.startsWith('#'))
        goto startOfLoop;

    line = GeneralTaskUtility::removeComment(line);

    for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
        if (!line.startsWith(it.key()))
            continue;
        if (!dictionary.value(it.key())(line, map))
            return "";

        for (int i = 0; i < requiredValuesNeeded.size(); ++i) {
            if (it.key() == requiredValuesNeeded[i])
                requiredValuesNeeded.remove(i, 1);
        }

        dictionary.erase(it);

        goto startOfLoop;
    }

    if (requiredValuesNeeded.size() == 0) {
        SceneDataModel::getInstance()->addElement(currentSceneName, type, map);
    }
    return line;
}

void ProjectReader::readDir(QString path)
{
    imagePath = path + "/images/";

    if (HTMLTemplateText == "") {
        QFile file{":/HTMLTemplate.html"};
        file.open(QFile::ReadOnly | QFile::Text);
        HTMLTemplateText = file.readAll();
        file.close();
    }

    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);

    QFile jsFile{":/markdownConverter.js"};
    jsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray byteArray = jsFile.readAll();
    jsFile.close();

    JSValue val = JS_Eval(ctx,
                          byteArray.constData(),
                          byteArray.size(),
                          ":/bundle.js",
                          JS_EVAL_TYPE_GLOBAL);

    JS_FreeValue(ctx, val);

    currentDir = new QDir(path);
    QFile file = currentDir->filePath("MAIN.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    GeneralTaskUtility::taskTypes type = GeneralTaskUtility::taskTypes::unknown;

    QTextStream in(&file);
    QString lineNeededToBeReadInstead{};
    while (!in.atEnd()) {
        QString line = lineNeededToBeReadInstead != "" ? lineNeededToBeReadInstead : in.readLine();
        lineNeededToBeReadInstead = "";
        if (type == GeneralTaskUtility::taskTypes::unknown) {
            if (line.endsWith(":::"))
                setNewSceneNameFromLine(line);
            else {
                type = getTaskTypeOfString(line);
            }
            if (type == GeneralTaskUtility::taskTypes::unknown)
                continue;
        }

        QString lastLine = generateObject(type, currentSceneName, in);
        if (lastLine != "") {
            lineNeededToBeReadInstead = lastLine;
        }
        type = GeneralTaskUtility::taskTypes::unknown;
    }

    HTMLTemplateText = "";

    file.close();

    JS_FreeContext(ctx);
    ctx = nullptr;
    JS_RunGC(rt);
    JS_FreeRuntime(rt);
    rt = nullptr;
}
