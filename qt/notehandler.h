#ifndef NOTEHANDLER_H
#define NOTEHANDLER_H

#include <QObject>

#include "note.h"
#include "notelist.h"
#include "simplenotesync.h"

class NoteHandler : public QObject
{
    Q_OBJECT
public:
    explicit NoteHandler(const QString& user, const QString& password, QObject *parent = 0);
    ~NoteHandler();
signals:

public slots:
    void onSimplenoteAuthentication(QNetworkReply::NetworkError);
    void onSimplenoteListUpdate(QNetworkReply::NetworkError err);

public://functions

    NoteList *getNoteList();
    void updateNoteList();
    void updateNote(const Note&);

private://functions

    void updateNoteListSimplenote();
    void updateNoteListDisk();

private://members

    NoteList* mNoteList = nullptr;
    SimplenoteSync* mSimplenoteSync = nullptr;
    //DiskNoteSync mDiskSync;
};

#endif // NOTEHANDLER_H
