#ifndef TRASHFILTERPROXYMODEL_H
#define TRASHFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

enum struct DisplayMode{
    OnlyNonTrashed,
    OnlyTrashed,
    Both
};

class TrashFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TrashFilterProxyModel(QObject* parent = 0, DisplayMode defaultMode = DisplayMode::OnlyNonTrashed);

    /**
     * @brief setViewMode This function sets the note display mode
     * @param mode either OnlyNonTrahed, OnlyTrashed or both
     */
    void setViewMode(DisplayMode mode);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &index) const override;

private:
    DisplayMode mCurrentDisplayMode;
};

#endif // TRASHFILTERPROXYMODEL_H
