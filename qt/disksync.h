#ifndef DISKSYNC_H
#define DISKSYNC_H

#include <qvector.h>

class Note;
class QString;

class DiskSync
{
public:
    /**
     * @brief saveToDisk save a list of notes to disk
     * @param noteList the notes to save
     */
    static void saveToDisk(const QVector<Note>& noteList);

    /**
     * @brief readFromDisk Read json object from disk
     * @return list of notes
     */
    static QVector<Note*>* readFromDisk();
};

#endif // DISKSYNC_H
