#include <QStack>
#include <QTimer>
#include <algorithm>

#include <assert.h>

#include "disksync.h"
#include "notelist.h"
#include "simplenotesync.h"

NoteList::NoteList(QObject* parent) : QAbstractListModel(parent) {
    // create a simplenote sync object; it is going to handle syncing with simplenote for us
    mSimplenoteSync = new SimplenoteSync(parent);

    // connect to the simplenote auth finish signal
    connect(mSimplenoteSync, SIGNAL(onAuthentication(QNetworkReply::NetworkError)),
            this, SLOT(onSimplenoteAuthentication(QNetworkReply::NetworkError)));

    // connect to the simplenote note list update finish signal
    connect(mSimplenoteSync, SIGNAL(onNoteList(QNetworkReply::NetworkError, QVector<Note*>*)),
            this, SLOT(onSimplenoteListUpdate(QNetworkReply::NetworkError, QVector<Note*>*)));

    // connect to the simplenote note list update finish signal
    connect(mSimplenoteSync, SIGNAL(onNote(QNetworkReply::NetworkError, Note*)),
            this, SLOT(onSimplenoteNoteFetched(QNetworkReply::NetworkError, Note*)));
}

/*
 *
 *
 *
 * PUBLIC INTERFACE
 *
 *
 *
 *
 */

void NoteList::fetchNoteList() {
    readFromDisk();
    mSimplenoteSync->fetchNoteList();
}

void NoteList::updateNote(const Note& n) {
    mSimplenoteSync->updateNote(n);
}

void NoteList::trashNote(Note &n, bool trash) {
    mSimplenoteSync->trashNote(n, trash);
}

void NoteList::writeToDisk()
{
    DiskSync::saveToDisk(mNoteList);
}

void NoteList::readFromDisk()
{
    onSimplenoteListUpdate(QNetworkReply::NetworkError::NoError, DiskSync::readFromDisk());
}

/*
 *
 *
 * PRIVATE UTILITY FUNCTIONS
 *
 *
 *
 */

#include <QThread>

void NoteList::fetchNote()
{
    if(!mCurrentlyFetchingNote && !mFetchStack.isEmpty()) {
        // save state
        mCurrentlyFetchingNote = true;

        QString key = mFetchStack.top();

        qDebug() << "notelist: fetching " << key;

        // fetch next note
        mSimplenoteSync->fetchNote(key);

        mFetchStack.pop();

        qDebug() << "remaining: " << mFetchStack.size();
    }
    else{
        if(!mFetchStack.isEmpty())
            qDebug() << "stack size: " << mFetchStack.size() << "currently fetching note: " << mCurrentlyFetchingNote;
    }
}



void NoteList::fetchNote(int row) {
    // check row ok
    assert(row <= mNoteList.size() && row >= 0);
    // fetch the note
    fetchNote(mNoteList.at(row));
}

void NoteList::fetchNote(const Note &note)
{
    // check whether the stack already contains this note
    if(!mFetchStack.contains(note.getKey())){
        // push the note onto the stack
        mFetchStack.push(note.getKey());
        // fetch the note just pushed onto the stack
        fetchNote();
    }
}

void NoteList::fetchNote(const QModelIndex &i) {
    // check index validity
    assert(i.isValid());
    // if the index is valid, begin fetching the note
    fetchNote(i.row());
}

void NoteList::insertUpdatedNotes(QVector<Note*>* updatedList) {
    // for each note in the updated list, check whether we already have a (newer) version.
    for(auto it = updatedList->begin(); it != updatedList->end(); it++) {
        // save the current note key for lambda capture
        const QString& currentNoteKey = (*it)->getKey();

        // check whether a note with this key exists
        auto result = std::find_if(mNoteList.begin(), mNoteList.end(), [currentNoteKey] (const Note& a) -> bool { return a.getKey() == currentNoteKey; });

        // if it does not exist in the current list, we must add it
        if(result == mNoteList.end()) {
            // notify the list of our plan
            beginInsertRows(QModelIndex(), mNoteList.size(), mNoteList.size() + 1);
            // add note
            mNoteList.append(**it);
            // fetch content if it was not loaded
            if(!(*it)->contentHasBeenFetched())
                fetchNote(mNoteList.last());
            // add process finished
            endInsertRows();
        }
        // if it exists, we check the version to decide whether or not to update it
        else {
            if((*it)->getVersion() > result->getVersion())
                fetchNote(findNote(currentNoteKey));
        }
    }
    //delete updatedList;
}

int NoteList::findNote(const QString &key) {
    // find the note
    auto keyPos = std::find_if(mNoteList.begin(), mNoteList.end(), [key] (const Note& a) -> bool { return a.getKey() == key; });

    // key not found?
    if(keyPos == mNoteList.end())
        return -1;

    // key found; return distance to beginning
    return std::distance(mNoteList.begin(), keyPos);
}

/*
 *
 *
 * SLOTS
 *
 *
 */

void NoteList::onSimplenoteAuthentication(QNetworkReply::NetworkError err) {
    emit onAuthentication(err);
}

void NoteList::onSimplenoteListUpdate(QNetworkReply::NetworkError, QVector<Note*>* noteList) {
    // parse note list from simplenote sync thingy
    if(noteList)
        insertUpdatedNotes(noteList);
}

void NoteList::onSimplenoteNoteFetched(QNetworkReply::NetworkError, Note* note) {
    qDebug() << "note " << (note ? "" : "in") << "valid";
    // abort if the note is invalid
    if(!note) return;

    // try and find the note
    int notePosition = findNote(note->getKey());

    // update our data store
    mNoteList.replace(notePosition, *note);

    // update the row text
    dataChanged(index(notePosition, 0, QModelIndex()), index(notePosition, 0, QModelIndex()));

    // call note fetched to signal the content update
    emit noteFetched(*note);

    // set state update
    mCurrentlyFetchingNote = false;

    // see if we need to fetch another note
    fetchNote();
}

void NoteList::onNoteClicked(QModelIndex index) {
    fetchNote(index);
}


/*
 *
 * INTERFACE IMPLEMENTATION
 *
 */

int NoteList::rowCount(const QModelIndex &) const {
    // return the note count
    return mNoteList.count();
}

QVariant NoteList::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.row() >= mNoteList.count())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    const Note& at = mNoteList.at(index.row());

    if(!at.contentHasBeenFetched())
        return "content not fetched";

    QString debugInfo;

    if(at.isDeleted())
        debugInfo += "<<trashed>>";

    return debugInfo + mNoteList.at(index.row()).getHeader();
}
