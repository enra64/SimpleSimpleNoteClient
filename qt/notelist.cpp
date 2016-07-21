#include "notelist.h"

NoteList::NoteList(QObject* parent) : QAbstractListModel(parent)
{

}

void NoteList::updateNoteList(QVector<Note*>* updatedList)
{
    // for each note in the updated list, check whether we already have a (newer) version.
    for(auto it = updatedList->begin(); it != updatedList->end(); it++){
        // save the current note key for lambda capture
        const QString& currentNoteKey = (*it)->getKey();

        // check whether a note with this key exists
        auto result = std::find_if(mNoteList.begin(), mNoteList.end(), [currentNoteKey] (Note a) -> bool { return a.getKey() == currentNoteKey; });

        // if it does not exist in the current list, we must add it
        if(result == mNoteList.end()){
            // notify the list of our plan
            beginInsertRows(QModelIndex(), mNoteList.size(), mNoteList.size() + 1);
            // add note
            mNoteList.push_back(**it);
            // add process finished
            endInsertRows();
        }
        // if it exists, we check the modifyDate to decide whether or not to update it
        else{

        }
    }
    delete updatedList;
}


int NoteList::rowCount(const QModelIndex &) const
{
    return mNoteList.count();
}

QVariant NoteList::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= mNoteList.count())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    const Note& at = mNoteList.at(index.row());

    if(!at.contentHasBeenFetched())
        return "content not fetched";

    return mNoteList.at(index.row()).getContent();
}

QVariant NoteList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}
