#include "settingsdialog.h"
#include "simplenotesync.h"
#include "ui_settingsdialog.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // remove second tab because i cant find how to in designer
    ui->tabWidget->removeTab(1);

    // load old settings
    QSettings settings;

    // set user field
    QString user = settings.value("user").toString();
    ui->userField->setText(user);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete mSync;
}

void SettingsDialog::on_buttonBox_accepted()
{
}

void SettingsDialog::tokenRetrieved(QNetworkReply::NetworkError err)
{
    if(err != QNetworkReply::NetworkError::NoError){
        // notify user of error
        ui->tokenButton->setText("network error; wrong credentials?");
    }
    else{
        // save data
        QSettings settings;
        settings.setValue("token", mSync->getToken());
        settings.setValue("user", ui->userField->text());

        // update status
        ui->tokenButton->setText("token retrieved");
    }
}

void SettingsDialog::on_tokenButton_clicked()
{
    // create simplenote sync object
    mSync = new SimplenoteSync(ui->userField->text(), ui->passwordField->text());

    // notify user of wait ahead
    ui->tokenButton->setText("please wait, retrieving token");

    // connect to auth signal
    connect(mSync, SIGNAL(onAuthentication(QNetworkReply::NetworkError)),
            this, SLOT(tokenRetrieved(QNetworkReply::NetworkError)));

    // call authenticate to retrieve a token
    mSync->authenticate();
}
