#include "trashfilterproxymodel.h"
#include "notelist.h"
#include "assert.h"

TrashFilterProxyModel::TrashFilterProxyModel(QObject *parent, DisplayMode defaultMode) :
    QSortFilterProxyModel(parent),
    mCurrentDisplayMode(defaultMode)
{

}

void TrashFilterProxyModel::setViewMode(DisplayMode mode)
{
    mCurrentDisplayMode = mode;
    invalidateFilter();
}

bool TrashFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & sourceParent) const
{
    // if no filter is set, our work is done here
    if(mCurrentDisplayMode == DisplayMode::Both)
        return true;

    // convert index so we can check for validity
    QModelIndex index = sourceModel()->index(source_row, 0, sourceParent);

    // abort if an invalid index was supplied
    if(!index.isValid())
        return false;

    // cast sourceModel to notelist, because we need more model data than data() gets
    NoteList* sourceNoteList = static_cast<NoteList*>(sourceModel());

    // assert cast ok
    assert(sourceNoteList != nullptr);

    // retrieve information about note trash status
    bool noteIsTrashed = sourceNoteList->mNoteList.at(source_row).isDeleted();

    // only show non trashed, but the note is trashed -> deny
    if(mCurrentDisplayMode == DisplayMode::OnlyNonTrashed && noteIsTrashed)
        return false;

    // only show trashed, but the note is not trashed -> deny
    if(mCurrentDisplayMode == DisplayMode::OnlyTrashed && !noteIsTrashed)
        return false;

    // in all other cases, accept the note
    return true;
}
