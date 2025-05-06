#include "flashcard.h"

FlashCard::FlashCard(QObject *parent)
    : QAbstractListModel(parent) {

    m_data = {
        {"Was ist 1+1?", "2"},
        {"Was ist 2+2?", "4"},
        {"Was ist 3+3?", "6"},
        {"Was ist 4+4?", "8"},
        {"Was ist 5+5", "10"},
        {"Was ist 6+6?", "12"},
        {"Was ist 7+7?", "14"},
        {"Was ist 8+8?", "16"},
        {"Was ist 9+9?", "18"},
        {"Was ist 10+10", "20"}
    };
}

int FlashCard::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}


QVariant FlashCard::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (row < 0 || row > m_data.count()) {
        return QVariant();
    }

    const auto &entry = m_data.at(index.row());

    switch(role) {
        case QuestionRole:
            return entry.first;
        case AnswerRole:
            return entry.second;
    }

    return QVariant();
}

QHash<int, QByteArray> FlashCard::roleNames() const {
    return {
        { QuestionRole, "question" },
        { AnswerRole, "answer" }
    };
}
