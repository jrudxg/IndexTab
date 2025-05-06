#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QtCore>
#include <QtGui>


class FlashCard: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FlashCard(QObject *parent = nullptr);

    enum FlashRoles {
        QuestionRole = Qt::UserRole + 1,
        AnswerRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
private:
    QList<QPair<QString, QString>> m_data;
protected:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FLASHCARD_H
