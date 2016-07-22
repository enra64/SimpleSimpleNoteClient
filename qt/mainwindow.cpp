#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // create a new notehandler object
    mNoteList = new NoteList("***REMOVED***", "***REMOVED***", this);

    ui->listView->setModel(mNoteList);

    // listen to click signals
    connect(ui->listView, SIGNAL(clicked(QModelIndex)),
            mNoteList, SLOT(onNoteClicked(QModelIndex)));

    // listen to note fetch ok signals
    connect(mNoteList, SIGNAL(noteFetched(const Note&)),
            this, SLOT(onNoteFetched(const Note&)));
}



MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    // try to authenticate with the simplenote service
    mNoteList->fetchNoteList();
}

void MainWindow::onAuthentication(QNetworkReply::NetworkError) {
    ui->pushButton->setText("this was not supposed to happen");
}

void MainWindow::onNoteFetched(const Note &note)
{
    ui->textBrowser->setText(note.getContent());
    mCurrentEditNote = new Note(note);
}

void MainWindow::on_actionSync_up_triggered()
{
    mCurrentEditNote->setContent(ui->textBrowser->toPlainText());
    if(mCurrentEditNote)
        mNoteList->updateNote(*mCurrentEditNote);
}
