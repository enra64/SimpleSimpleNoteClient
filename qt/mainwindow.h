#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QModelIndex>
#include <QMainWindow>
#include <QNetworkReply>

class TrashFilterProxyModel;
class NoteList;
class Note;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
private slots://note stuff
    void onAuthentication(QNetworkReply::NetworkError);
    void onNoteFetched(const Note& note);

private slots://ui callbacks
    void on_actionSync_up_triggered();
    void onNoteClicked(QModelIndex index);
    void on_actionDelete_triggered();

    /**
     * @brief onTrashNote Call to move the current note to the trash
     */
    void onTrashNote();

    /**
     * @brief onToggleTrashView this slot sets the visibility of notes - either only non trashed or both
     * @param enable true if the trash should be shown
     */
    void on_actionToggle_Showing_Trash_triggered(bool enable);

private://functions
    void closeEvent(QCloseEvent *bar) override;

private://members
    NoteList* mNoteList;
    TrashFilterProxyModel* mTrashFilterProxyModel;
    Ui::MainWindow *ui;
    Note* mCurrentEditNote = nullptr;
};

#endif // MAINWINDOW_H
