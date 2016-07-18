#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simplenotesync.h"

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
    Ui::MainWindow *ui;
    SimplenoteSync* mSimplenoteSync;
};

#endif // MAINWINDOW_H
