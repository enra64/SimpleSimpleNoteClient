#include "simplenotesync.h"

#include <QJsonDocument>

SimplenoteSync::SimplenoteSync(const QString& user, const QString& password, QObject *parent) : QObject(parent) {
    mUser = user;
    mPassword = password;
    mToken = "";

    mNetworkManager = new QNetworkAccessManager(this);
}

SimplenoteSync::~SimplenoteSync()
{
    delete mNetworkManager;
}

void SimplenoteSync::authenticate() {
    if(!mToken.isEmpty())
        return;

    // create post data string
    QString authenticationParameters = QString("email=%1&password=%2").arg(mUser, mPassword);

    // encode the bytes from the utf-8 encoded string using base64
    QByteArray parameterPostData = authenticationParameters.toUtf8().toBase64();

    // create a request with the appropriate URL
    QNetworkRequest authRequest(AUTH_URL);

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(authenticationRequestFinished(QNetworkReply*)));

    // start the request via POST
    mNetworkManager->post(authRequest, parameterPostData);
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

void SimplenoteSync::getNoteList() {
    // create post data string
    QString postString = QString("auth=%1&email=%2&length=100").arg(getToken(), mUser);

    QNetworkRequest listRequest(postString.prepend(INDEX_URL));

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(noteListRequestFinished(QNetworkReply*)));

    // start the request via GET
    mNetworkManager->get(listRequest);
}

void SimplenoteSync::noteListRequestFinished(QNetworkReply *reply)
{
    QJsonDocument data;
    // check whether any errors occured
    if(reply->error() != QNetworkReply::NoError){
        QString replyData = reply->readAll();
        data = QJsonDocument::fromJson(reply->readAll());
    }
    onNoteList(reply->error());
}

QString SimplenoteSync::getToken()
{
    // call authenticate to request a token if none exists
    authenticate();

    // after the authenticate() call we should have a token
    return mToken;
}

Note SimplenoteSync::getNote(Note &) {
    return Note("invalid");
}

Note SimplenoteSync::getNote(const QString &) {
    return Note("invalid");
}

void SimplenoteSync::updateNote(Note &) {

}

void SimplenoteSync::addNote(Note &) {

}



void SimplenoteSync::deleteNote(Note &) {

}

void SimplenoteSync::trashNote(Note &) {

}
