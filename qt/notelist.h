#ifndef NOTELIST_H
#define NOTELIST_H

#include <QAbstractListModel>
#include <QNetworkReply>
#include <QMap>

#include <algorithm>

#include "note.h"
#include "simplenotesync.h"

class NoteList : public QAbstractListModel
{
    Q_OBJECT
public:
    NoteList(const QString &user, const QString &password, QObject *parent = 0);

    /**
     * @brief fetchNote Fetches the complete note, and updates it
     * @param i index of the note
     */
    void fetchNote(const QModelIndex& i);

    void updateNoteList();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


public slots:
    /**
     * @brief onSimplenoteAuthentication This slot is for the "auth request finished"-signal from the simplenote adapter
     */
    void onSimplenoteAuthentication(QNetworkReply::NetworkError);

    /**
     * @brief onSimplenoteListUpdate This slot is for the "list request finished"-signal from the simplenote adapter
     * @param noteList the list of notes, to be deleted by the receiver.
     */
    void onSimplenoteListUpdate(QNetworkReply::NetworkError, QVector<Note *> *noteList);

    /**
     * @brief onSimplenoteListUpdate This slot is for the "list request finished"-signal from the simplenote adapter
     * @param noteList the list of notes, to be deleted by the receiver.
     */
    void onSimplenoteNoteUpdate(QNetworkReply::NetworkError, Note* note);

    /**
     * @brief onNoteClicked To be called when a note has been clicked
     * @param index index of the clicked note
     */
    void onNoteClicked(QModelIndex index);

private://functions

    /**
     * @brief updateNoteList iterate through the list, checking whether we should update anything
     * @param updatedList the updated list
     */
    void updateNoteList(QVector<Note*>* updatedList);

    /**
     * @brief findNote Find the index of a note
     * @param key key of the note in question
     * @return index, or -1 if the note was not found
     */
    int findNote(const QString& key);

private://members
    QVector<Note> mNoteList;
    SimplenoteSync* mSimplenoteSync = nullptr;
};

#endif // NOTELIST_H
