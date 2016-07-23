#ifndef NOTELIST_H
#define NOTELIST_H

#include <QAbstractListModel>
#include <QNetworkReply>

#include <algorithm>

#include "simplenotesync.h"
#include "note.h"

class NoteList : public QAbstractListModel
{
    Q_OBJECT
public:
    // declare our proxy model as friend to be able to access the data
    friend class TrashFilterProxyModel;

    NoteList(const QString &user, const QString &password, QObject *parent = 0);

    /**
     * @brief fetchNote Fetches the note content, and updates it
     * @param i index of the note
     */
    void fetchNote(const QModelIndex& i);

    /**
     * @brief fetchNoteList Fetch a list of available notes
     */
    void fetchNoteList();

    /**
     * @brief updateNote Call this to synchronize a note with the server version
     * @param newContent the new note content
     */
    void updateNote(const Note &n);

    /**
     * @brief trashNote Call this to move a note to the trash
     * @param n the note to trash
     */
    void trashNote(Note &n, bool trash = true);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;

signals:
    /**
     * @brief noteFetched This signal is issued when a note has been fetched, indicating that the note content has been loaded
     * @param note the note that was received
     */
    void noteFetched(const Note& note);

public slots:
    /**
     * @brief onNoteClicked To be called when a note has been clicked
     * @param index index of the clicked note
     */
    void onNoteClicked(QModelIndex index);

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
    void onSimplenoteNoteFetched(QNetworkReply::NetworkError, Note* note);

private://functions

    /**
     * @brief updateNoteList iterate through the list, checking whether we should update anything
     * @param updatedList the updated list
     */
    void insertUpdatedNotes(QVector<Note*>* updatedList);

    /**
     * @brief findNote Find the index of a note
     * @param key key of the note in question
     * @return index, or -1 if the note was not found
     */
    int findNote(const QString& key);

private://members

    /**
     * @brief mNoteList Vector used to store all note objects
     */
    QVector<Note> mNoteList;

    /**
     * @brief mSimplenoteSync pointer to our simplenote sync object
     */
    SimplenoteSync* mSimplenoteSync = nullptr;
};

#endif // NOTELIST_H
