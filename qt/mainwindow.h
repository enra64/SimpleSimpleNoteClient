#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "notelist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
private slots:
    void onAuthentication(QNetworkReply::NetworkError);
    void onNoteFetched(const Note& note);

private slots://ui callbacks
    void on_pushButton_clicked();
    void on_actionSync_up_triggered();

    void on_actionDelete_triggered();

private:
    NoteList* mNoteList;
    Ui::MainWindow *ui;
    Note* mCurrentEditNote = nullptr;
};

#endif // MAINWINDOW_H
