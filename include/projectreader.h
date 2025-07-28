#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include <QDir>
#include <QObject>
#include "generaltaskutility.h"


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
