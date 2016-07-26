#ifndef NOTELIST_H
#define NOTELIST_H

#include <QAbstractListModel>
#include <QNetworkReply>
#include <QStack>

#include "simplenotesync.h"
#include "note.h"

class NoteList : public QAbstractListModel
{
    Q_OBJECT
public:
    // declare our proxy model as friend to be able to access the data
    friend class TrashFilterProxyModel;

    NoteList(const QString &user, const QString &password, const QString& dataPath, QObject *parent = 0);



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

    /**
     * @brief writeToDisk write out the current notelist to disk
     */
    void writeToDisk();

    /**
     * @brief readFromDisk read the notelist from disk, and update our store
     */
    void readFromDisk();

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

    /**
     * @brief onAuthentication This signal is issued when the auth request has been answered.
     * @param err the error that occured
     */
    void onAuthentication(QNetworkReply::NetworkError err);

public slots:
    /**
     * @brief onNoteClicked To be called when a note has been clicked
     * @param index index of the clicked note
     */
    void onNoteClicked(QModelIndex index);

    /**
     * @brief onSimplenoteAuthentication This slot is for the "auth request finished"-signal from the simplenote adapter
     */
    void onSimplenoteAuthentication(QNetworkReply::NetworkError err);

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
     * @brief fetchNote Fetches the note content, and updates it
     * @param i index of the note
     */
    void fetchNote(const QModelIndex& i);

    /**
     * @brief fetchNote Fetches the note content, and updates it
     * @param i row of the note
     */
    void fetchNote(int row);

    /**
     * @brief fetchNote Fetches the note content, and updates it
     * @param i the note
     */
    void fetchNote(const Note& note);

    /**
     * @brief fetchNote Fetch the content of the note at the top of the stack
     */
    void fetchNote();

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
     * @brief mCurrentlyFetchingNote Variable used to determine whether we can currently fetch a note
     */
    bool mCurrentlyFetchingNote = false;

    /**
     * @brief mNoteList Vector used to store all note objects
     */
    QVector<Note> mNoteList;

    /**
     * @brief mFetchStack A stack of keys of notes that we still need to request
     */
    QStack<QString> mFetchStack;

    /**
     * @brief mSimplenoteSync pointer to our simplenote sync object
     */
    SimplenoteSync* mSimplenoteSync = nullptr;

    /**
     * @brief mDataPath the path where disk storage for notes lies
     */
    QString mDataPath;
};

#endif // NOTELIST_H
