#ifndef SIMPLENOTESYNC_H
#define SIMPLENOTESYNC_H

#include <QObject>
#include <QString>
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

    /**
     * @brief authenticate Get the saved API auth token, or retrieve one from the servers.
     */
    void authenticate();

    /**
     * @brief getToken Use this method if you want to retrieve the token currently saved.
     * @return current token
     */
    QString getToken();

    Note getNote(Note& note);
    Note getNote(const QString& noteId);
    void updateNote(Note& note);
    void addNote(Note& note);
    void getNoteList(void);
    void deleteNote(Note& note);
    void trashNote(Note& note);

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
    const QUrl INDEX_URL = QUrl("https://app.simplenote.com/api2/index?");

    QNetworkAccessManager* mNetworkManager = nullptr;

signals:
    void onAuthentication(QNetworkReply::NetworkError errorCode);

public slots:
    void authenticationRequestFinished(QNetworkReply* reply);
};

#endif // SIMPLENOTESYNC_H
