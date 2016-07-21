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
    void on_pushButton_clicked();
    void onAuthentication(QNetworkReply::NetworkError);

private:
    NoteList* mNoteList;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
