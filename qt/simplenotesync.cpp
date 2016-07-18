#include "simplenotesync.h"

SimplenoteSync::SimplenoteSync(const QString& user, const QString& password, QObject *parent) : QObject(parent) {
    mUser = user;
    mPassword = password;
    mToken = "";

    mNetworkManager = new QNetworkAccessManager(this);
}

void SimplenoteSync::authenticate() {
    if(!mToken.isEmpty())
        return;

    // create URL request
    QString authenticationParameters = QString("email=%1&password=%2").arg(mUser, mPassword);

    // encode the bytes from the utf-8 encoded string with base64
    QByteArray parameterPostData = authenticationParameters.toUtf8().toBase64();

    // create a request with the appropriate URL
    QNetworkRequest authRequest(AUTH_URL);

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(authenticationRequestFinished(QNetworkReply*)));

    // start the request via POST
    mNetworkManager->post(authRequest, parameterPostData);
}

QString SimplenoteSync::getToken()
{
    return mToken;
}

Note SimplenoteSync::getNote(Note &note) {
}

Note SimplenoteSync::getNote(const QString &noteId) {

}

void SimplenoteSync::updateNote(Note &note) {

}

void SimplenoteSync::addNote(Note &note) {

}

void SimplenoteSync::getNoteList() {

}

void SimplenoteSync::deleteNote(Note &note) {

}

void SimplenoteSync::trashNote(Note &note) {

}

void SimplenoteSync::authenticationRequestFinished(QNetworkReply *reply)
{
    // check whether any errors occured
    if(reply->error() != QNetworkReply::NoError)
        // invalidate the token
        mToken = "";
    else
        // save the token
        mToken = QString::fromUtf8(reply->readAll());

    // send a status signal
    onAuthentication(reply->error());
}
