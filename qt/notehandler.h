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
    /**
     * @brief onSimplenoteAuthentication This slot is for the "auth request finished"-signal from the simplenote adapter
     */
    void onSimplenoteAuthentication(QNetworkReply::NetworkError);

    /**
     * @brief onSimplenoteListUpdate This slot is for the "list request finished"-signal from the simplenote adapter
     * @param noteList the list of notes, to be deleted by the receiver.
     */
    void onSimplenoteListUpdate(QNetworkReply::NetworkError, QVector<Note *> *noteList);

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
