#ifndef DISKSYNC_H
#define DISKSYNC_H

#include <qvector.h>

class Note;
class QString;

class DiskSync
{
public:
    /**
     * @brief DiskSync create a DiskSync object
     * @param path the path where the storage file lies
     */
    DiskSync(const QString& path);

    /**
     * @brief saveToDisk save a list of notes to disk
     * @param noteList the notes to save
     */
    void saveToDisk(const QVector<Note>& noteList);

    /**
     * @brief readFromDisk Read json object from disk
     * @return list of notes
     */
    QVector<Note*>* readFromDisk();
private:
    /**
     * @brief mPath Contains the save/load path
     */
    QString mPath;
};

#endif // DISKSYNC_H
