#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include <QObject>

class ProjectReader : public QObject
{
    Q_OBJECT

public:
    explicit ProjectReader(QObject *parent = nullptr)
    : QObject{parent} {}

    void readFile(QString source);
private:

    QString currentSceneName;
    void setNewSceneNameFromLine(QString line);
    static const QRegularExpression removeTrippleColumns;
};

#endif // PROJECTREADER_H
