#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create a new notehandler object
    mNoteHandler = new NoteHandler("***REMOVED***", "***REMOVED***", this);

    ui->listView->setModel(mNoteHandler->getNoteList());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // try to authenticate with the simplenote service
    mNoteHandler->updateNoteList();
}

void MainWindow::onAuthentication(QNetworkReply::NetworkError)
{
    ui->pushButton->setText("this was not supposed to happen");
}
