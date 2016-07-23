#include "notelist.h"

#include "simplenotesync.h"

NoteList::NoteList(const QString &user, const QString &password, QObject* parent) : QAbstractListModel(parent) {
    // create a simplenote sync object; it is going to handle syncing with simplenote for us
    mSimplenoteSync = new SimplenoteSync(user, password, parent);

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

void NoteList::fetchNote(const QModelIndex &i) {
    mSimplenoteSync->fetchNote(mNoteList.at(i.row()));
}

void NoteList::fetchNoteList() {
    mSimplenoteSync->fetchNoteList();
}

void NoteList::updateNote(const Note& n) {
    mSimplenoteSync->updateNote(n);
}

void NoteList::trashNote(Note &n, bool trash) {
    mSimplenoteSync->trashNote(n, trash);
}

/*
 *
 *
 * PRIVATE UTILITY FUNCTIONS
 *
 *
 *
 */

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
            mNoteList.push_back(**it);
            // add process finished
            endInsertRows();
        }
        // if it exists, we check the modifyDate to decide whether or not to update it
        else {

        }
    }
    delete updatedList;
}

int NoteList::findNote(const QString &key) {
    // find the note
    auto keyPos = std::find_if(mNoteList.begin(), mNoteList.end(), [key] (Note a) -> bool { return a.getKey() == key; });

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

void NoteList::onSimplenoteAuthentication(QNetworkReply::NetworkError) {

}

void NoteList::onSimplenoteListUpdate(QNetworkReply::NetworkError, QVector<Note*>* noteList) {
    // parse note list from simplenote sync thingy
    if(noteList)
        insertUpdatedNotes(noteList);
}

void NoteList::onSimplenoteNoteFetched(QNetworkReply::NetworkError, Note* note) {
    // abort if the note is invalid
    if(!note) return;

    // try and find the note
    int notePosition = findNote(note->getKey());

    // update our data store
    mNoteList.replace(notePosition, *note);

    // _really_ update the row text
    dataChanged(index(notePosition, 0, QModelIndex()), index(notePosition, 0, QModelIndex()));

    // call note fetched to signal the content update
    emit noteFetched(*note);
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
