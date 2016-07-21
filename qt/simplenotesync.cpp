#include "simplenotesync.h"

#include <QEventLoop>
#include <QJsonDocument>

SimplenoteSync::SimplenoteSync(const QString& user, const QString& password, QObject *parent) : QObject(parent) {
    mUser = user;
    mPassword = password;
    mToken = "";

    mNetworkManager = new QNetworkAccessManager(this);
}

SimplenoteSync::~SimplenoteSync() {
    delete mNetworkManager;
}

void SimplenoteSync::authenticate() {
    mToken = "D45F9AC2493DE08C68C9C79A90570E735688EEC7C96390EF4C7882DC58F047F7";
    if(!mToken.isEmpty())
        return;

    // create post data string
    QString authenticationParameters = QString("email=%1&password=%2").arg(mUser, mPassword);

    // encode the bytes from the utf-8 encoded string using base64
    QByteArray parameterPostData = authenticationParameters.toUtf8().toBase64();

    // create a request with the appropriate URL
    QNetworkRequest authRequest(AUTH_URL);

    // create an qt event loop to wait unto the network request
    QEventLoop synchronizeRequestLoop;

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            &synchronizeRequestLoop, SLOT(quit()));

    // start the request via POST
    QNetworkReply* reply = mNetworkManager->post(authRequest, parameterPostData);

    // start waiting for the reply
    synchronizeRequestLoop.exec(QEventLoop::ExcludeUserInputEvents);

    // handle the reply
    authenticationRequestFinished(reply);
}

void SimplenoteSync::authenticationRequestFinished(QNetworkReply *reply) {
    // check whether any errors occured
    if(reply->error() != QNetworkReply::NoError)
        // invalidate the token
        mToken = "";
    else
        // save the token
        mToken = QString::fromUtf8(reply->readAll());

    // send a status signal
    onAuthentication(reply->error());
    std::cout << "auth finished: " << reply->isFinished() << "open?:" << reply->isOpen() << std::endl;

    // clean up
    reply->deleteLater();
}

void SimplenoteSync::getNote(const QString& key) {
    // create url data string
    QString postString = QString("%1?auth=%2&email=%3").arg(key, getToken(), mUser);

    // create a request from that
    QNetworkRequest listRequest(postString.prepend(DATA_URL));

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(noteRequestFinished(QNetworkReply*)));

    std::cout << postString.toStdString() << std::endl;

    // start the request via GET
    mNetworkManager->get(listRequest);
}

void SimplenoteSync::noteRequestFinished(QNetworkReply *reply)
{
    // check whether any errors occured
    if(reply->error() == QNetworkReply::NoError) {
        std::cout << reply->isFinished() << reply->isOpen() << std::endl;

        // create json object from response
        QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();

        // parse json data
        onNote(reply->error(), data.empty() ? nullptr : new Note(data));
    } else
        onNote(reply->error(), nullptr);

    // disconnect the finish signal to avoid double-reading
    disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(noteRequestFinished(QNetworkReply*)));

    // clean up
    reply->deleteLater();
}

void SimplenoteSync::getNoteList() {
    // create url data string
    QString postString = QString("auth=%1&email=%2&length=100").arg(getToken(), mUser);

    // create a request from that
    QNetworkRequest listRequest(postString.prepend(INDEX_URL));

    // connect to the finished signal
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(noteListRequestFinished(QNetworkReply*)));

    // start the request via GET
    mNetworkManager->get(listRequest);
}

void SimplenoteSync::noteListRequestFinished(QNetworkReply *reply) {
    // check whether any errors occured
    if(reply->error() == QNetworkReply::NoError) {
        // create json object from response
        QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
        // parse json data
        onNoteList(reply->error(), parseJsonToNotelist(data));
    } else
        onNoteList(reply->error(), nullptr);

    // disconnect the finish signal to avoid double-reading
    disconnect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(noteListRequestFinished(QNetworkReply*)));

    // clean up
    reply->deleteLater();
}

QVector<Note*>* SimplenoteSync::parseJsonToNotelist(const QJsonObject& json) {
    // initialise result data vector
    QVector<Note*>* noteVector = new QVector<Note*>();

    // access data array within json
    QJsonArray notes = json.value("data").toArray();

    // create a note object for each data array element
    for(int i = 0; i < notes.size(); i++)
        noteVector->push_back(new Note(notes.at(i)));

    // return result
    return noteVector;
}

QString SimplenoteSync::getToken() {
    // call authenticate to request a token if none exists
    authenticate();

    // after the authenticate() call we should have a token
    return mToken;
}

void SimplenoteSync::getNote(const Note & n) {
    getNote(n.getKey());
}

void SimplenoteSync::updateNote(Note &) {

}

void SimplenoteSync::addNote(Note &) {

}

void SimplenoteSync::deleteNote(Note &) {

}

void SimplenoteSync::trashNote(Note &) {

}
