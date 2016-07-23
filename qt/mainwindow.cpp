#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QModelIndex>

#include "notelist.h"
#include "note.h"
#include "trashfilterproxymodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // create a new notehandler object
    mNoteList = new NoteList("***REMOVED***", "***REMOVED***", this);


    mTrashFilterProxyModel = new TrashFilterProxyModel(this);

    mTrashFilterProxyModel->setSourceModel(mNoteList);
    ui->listView->setModel(mTrashFilterProxyModel);

    // listen to click signals on note list
    connect(ui->listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onNoteClicked(QModelIndex)));

    // listen to note fetch ok signals
    connect(mNoteList, SIGNAL(noteFetched(const Note&)),
            this, SLOT(onNoteFetched(const Note&)));

    // enable the trash toggle
    ui->mainToolBar->addAction("show trash only", this, SLOT(onToggleTrashView(bool)))->setCheckable(true);
}

void MainWindow::onToggleTrashView(bool enable)
{
    mTrashFilterProxyModel->setViewMode(enable ? DisplayMode::OnlyTrashed : DisplayMode::OnlyNonTrashed);
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
    if(mCurrentEditNote){
        mCurrentEditNote->setContent(ui->textBrowser->toPlainText());
        mNoteList->updateNote(*mCurrentEditNote);
    }
}

void MainWindow::onNoteClicked(QModelIndex index)
{
    // map the filtered index to the unfiltered, and then call notelist
    mNoteList->onNoteClicked(mTrashFilterProxyModel->mapToSource(index));
}

void MainWindow::on_actionDelete_triggered()
{
    if(mCurrentEditNote)
        mNoteList->trashNote(*mCurrentEditNote);
}
