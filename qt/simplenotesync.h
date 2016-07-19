#ifndef SIMPLENOTESYNC_H
#define SIMPLENOTESYNC_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <iostream>

#include "note.h"

class SimplenoteSync : public QObject
{
    Q_OBJECT
public:
    //explicit SimplenoteSync(QObject *parent = 0);
    /**
     * @brief SimplenoteSync Create a new SimplenoteSync instance. To initialise, call authenticate!
     * @param user username to use for login
     * @param password password to use for login
     */
    explicit SimplenoteSync(const QString &user, const QString &password, QObject *parent = 0);

    ~SimplenoteSync();

    /**
     * @brief authenticate Get the saved API auth token, or retrieve one from the servers.
     */
    void authenticate();

    /**
     * @brief getToken Use this method if you want to retrieve the token currently saved.
     * @return current token
     */
    QString getToken();

    Note getNote(Note&);
    Note getNote(const QString&);
    void updateNote(Note&);
    void addNote(Note&);
    void getNoteList(void);
    void deleteNote(Note&);
    void trashNote(Note&);

private:
    QString mUser, mPassword, mToken;

    /**
     * @brief AUTH_URL Authentication URL constant
     */
    const QUrl AUTH_URL = QUrl("https://app.simplenote.com/api/login");

    /**
     * @brief DATA_URL Data URL constant
     */
    const QUrl DATA_URL = QUrl("https://app.simplenote.com/api2/data");

    /**
     * @brief INDX_URL Index URL constant
     */
    const QString INDEX_URL = "https://app.simplenote.com/api2/index?";

    QNetworkAccessManager* mNetworkManager = nullptr;

signals:
    /**
     * @brief onAuthentication This signal is emitted after auth request replies have been handled
     * @param errorCode The error code received with the reply; NoError if reply was ok.
     */
    void onAuthentication(QNetworkReply::NetworkError errorCode);

    /**
     * @brief onNoteList This signal is emitted after note list update replies have been handled
     * @param errorCode The error code received with the reply; NoError if reply was ok.
     */
    void onNoteList(QNetworkReply::NetworkError errorCode);

public slots:
    /**
     * @brief authenticationRequestFinished This slot is to be connected with finished-signals for authentication requests
     * @param reply The reply received for the auth request
     */
    void authenticationRequestFinished(QNetworkReply* reply);

    /**
     * @brief noteListRequestFinished This slot is to be connected with finished-signals for note list requests
     * @param reply The reply received for the note list request
     */
    void noteListRequestFinished(QNetworkReply* reply);

};

#endif // SIMPLENOTESYNC_H
