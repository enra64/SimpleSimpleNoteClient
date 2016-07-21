#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create a new notehandler object
    mNoteList = new NoteList("***REMOVED***", "***REMOVED***", this);

    ui->listView->setModel(mNoteList);

    ui->listView->connect(ui->listView, SIGNAL(clicked(QModelIndex)), mNoteList, SLOT(onNoteClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // try to authenticate with the simplenote service
    mNoteList->updateNoteList();
}

void MainWindow::onAuthentication(QNetworkReply::NetworkError)
{
    ui->pushButton->setText("this was not supposed to happen");
}
