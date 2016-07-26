#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QModelIndex>

#include "notelist.h"
#include "note.h"
#include "trashfilterproxymodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // create a new notelist
    mNoteList = new NoteList("***REMOVED***",
                             "***REMOVED***",
                             "/home/arne/test.json", this);

    //
    mTrashFilterProxyModel = new TrashFilterProxyModel(this);

    // set our notelist as proxy source
    mTrashFilterProxyModel->setSourceModel(mNoteList);

    // set the proxy model as the list model
    ui->listView->setModel(mTrashFilterProxyModel);

    // listen to click signals on note list
    connect(ui->listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onNoteClicked(QModelIndex)));

    // listen to note fetch ok signals
    connect(mNoteList, SIGNAL(noteFetched(const Note&)),
            this, SLOT(onNoteFetched(const Note&)));

    // listen to auth signals
    //connect(mNoteList, SIGNAL(onAuthentication()), this, SLOT(onAuthentication()));

    // enable the trash button
    QAction* trashAction = ui->mainToolBar->addAction("Delete note", this, SLOT(onTrashNote()));

    // set trash icon
    trashAction->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));

    // fetch the note list
    mNoteList->fetchNoteList();
}

void MainWindow::onTrashNote()
{
    on_actionDelete_triggered();
}

void MainWindow::on_actionToggle_Showing_Trash_triggered(bool enable)
{
    mTrashFilterProxyModel->setViewMode(enable ? DisplayMode::Both : DisplayMode::OnlyNonTrashed);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mNoteList->writeToDisk();
    event->accept();
}

MainWindow::~MainWindow() {
    delete mNoteList;
    delete mCurrentEditNote;
    delete mTrashFilterProxyModel;
    delete ui;
}

void MainWindow::onAuthentication(QNetworkReply::NetworkError) {
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


