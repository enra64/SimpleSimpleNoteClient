#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QNetworkReply>

class SimplenoteSync;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    /**
     * @brief tokenRetrieved slot to react to authentication
     */
    void tokenRetrieved(QNetworkReply::NetworkError err);
    void on_tokenButton_clicked();

private:
    Ui::SettingsDialog *ui;

    SimplenoteSync* mSync;
};

#endif // SETTINGSDIALOG_H
