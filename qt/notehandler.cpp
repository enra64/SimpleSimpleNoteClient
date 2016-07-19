#include "notehandler.h"

NoteHandler::NoteHandler(const QString &user, const QString &password, QObject *parent) : QObject(parent)
{
    // create a noteList object; it is going to save and handle the notes for us
    mNoteList = new NoteList(this);

    // create a simplenote sync object; it is going to handle syncing with simplenote for us
    mSimplenoteSync = new SimplenoteSync(user, password, parent);

    // connect to the simplenote auth finish signal
    connect(mSimplenoteSync, SIGNAL(onAuthentication(QNetworkReply::NetworkError)),
            this, SLOT(onSimplenoteAuthentication(QNetworkReply::NetworkError)));

    // connect to the simplenote note list update finish signal
    connect(mSimplenoteSync, SIGNAL(onNoteList(QNetworkReply::NetworkError)),
            this, SLOT(onSimplenoteListUpdate(QNetworkReply::NetworkError)));
}

NoteHandler::~NoteHandler()
{

}

void NoteHandler::onSimplenoteAuthentication(QNetworkReply::NetworkError)
{

}

void NoteHandler::onSimplenoteListUpdate(QNetworkReply::NetworkError err)
{
    // todo: somehow achieve intelligent behaviour when fetching the note list. currently a mismatch between the expected
    // QVECtor<note> and the parsed notelist exists
    // parse note list from simplenote sync thingy
    mNoteList->updateNoteList(mSimplenoteSync->getFetchedNoteList());
}

NoteList* NoteHandler::getNoteList()
{
    return mNoteList;
}

void NoteHandler::updateNoteList()
{
    updateNoteListSimplenote();
}

void NoteHandler::updateNoteListSimplenote()
{
    mSimplenoteSync->getNoteList();
}

void NoteHandler::updateNoteListDisk()
{

}


void NoteHandler::updateNote(const Note &)
{

}

