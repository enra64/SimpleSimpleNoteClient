#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create a simplenote sync object
    mSimplenoteSync = new SimplenoteSync(, this);

    // connect to the auth signal
    connect(mSimplenoteSync, SIGNAL(onAuthentication(QNetworkReply::NetworkError)),
            this, SLOT(onAuthentication(QNetworkReply::NetworkError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // try to authenticate with the simplenote service
    mSimplenoteSync->authenticate();
}

void MainWindow::onAuthentication(QNetworkReply::NetworkError)
{
    QString token = mSimplenoteSync->getToken();
    ui->pushButton->setText(token);
}
