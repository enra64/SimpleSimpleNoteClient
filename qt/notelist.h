#ifndef NOTELIST_H
#define NOTELIST_H

#include <QAbstractListModel>
#include <QMap>

#include <algorithm>

#include "note.h"

class NoteList : public QAbstractListModel
{
    Q_OBJECT
public://functions
    NoteList(QObject *parent = 0);

    /**
     * @brief updateNoteList iterate through the list, checking whether we should update anything
     * @param updatedList the updated list
     */
    void updateNoteList(QVector<Note*>* updatedList);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QVector<Note> mNoteList;
};

#endif // NOTELIST_H
