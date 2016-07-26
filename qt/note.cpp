#include "note.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

#include <assert.h>

const QVector<QString> &Note::getTags() {
    return mTags;
}

const QVector<QString> &Note::getSystemTags() {
    return mSystemTags;
}

const QString &Note::getKey() const {
    return mKey;
}

QString Note::getContent() {
    return mContent;
}

QString Note::getHeader() const {
    // default to breaking after this amount of characters
    int breakPosition = 40;

    // try to break at the line break
    int lineBreakPosition = mContent.indexOf('\n');

    // a line break was found - cut off here
    if(lineBreakPosition != -1)
        breakPosition = lineBreakPosition;

    // return the substring up to the break position
    return mContent.left(breakPosition);
}

const QString &Note::getContent() const {
    return mContent;
}

const QDateTime &Note::getCreateDate() {
    return mCreateDate;
}

const QDateTime &Note::getModifyDate() {
    return mModifyDate;
}

bool Note::isDeleted() const {
    return mIsDeleted;
}

int Note::getVersion() const {
    return mVersion;
}

int Note::getMinVersion() const {
    return mMinVersion;
}

int Note::getSyncnum() const {
    return mSyncnum;
}

bool Note::isNewNote() const {
    return mKey.isEmpty();
}

void Note::setContent(const QString &newContent) {
    mContent = newContent;
}

void Note::setTags(const QVector<QString> &) {
    assert(false);
}

void Note::setModifyDate(const QDateTime &timestamp) {
    mModifyDate = timestamp;
}

void Note::setDeleted(bool del) {
    mIsDeleted = del;
}

QJsonDocument Note::jsonDump(bool insertModificationTimestamp) const {
    // data object
    QJsonObject data;

    // ignore if new note
    if(!mKey.isEmpty())
        data.insert("key", QJsonValue(mKey));

    data.insert("content", QJsonValue(mContent));

    // the delete field is used to trash notes.
    data.insert("deleted", QJsonValue(mIsDeleted ? 1 : 0));// need int representation for bool

    // version field for comparing notes on disk to net
    data.insert("version", QJsonValue(mVersion));

    // the modification field is used (i think) for the mod dates shown on the server
    if(insertModificationTimestamp)
        data.insert("modifydate", QJsonValue(QString::number(QDateTime::currentDateTimeUtc().toTime_t()).append(".719370")));// current unix timestamp + wild guess about what sn wants

    // return as plaintext json
    return QJsonDocument(data);
}

QVector<Note*>* Note::parseJsonToNotelist(const QJsonObject& json) {
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

bool Note::contentHasBeenFetched() const {
    return mContentFetched;
}

void Note::deleteKey() {
    mKey = "";
}

QDateTime Note::getTime(const QJsonValue &from) {
    // get data as string
    QString raw = from.toString();
    // truncate to integer part of time
    QString integerPart = raw.remove(raw.indexOf('.'), raw.length());
    // parse & return
    return QDateTime::fromTime_t(integerPart.toInt());
}

void Note::parseJsonToVector(const QJsonValue &json, QVector<QString> &target) {
    // create array from tags
    QJsonArray tagArray = json.toArray();
    // push all tags into the targets
    for(auto i : tagArray) target.push_back(i.toString());
}

QJsonValue Note::parseVectorToJson(const QVector<QString>& source) const {
    // create new json array
    QJsonArray data;
    // add all tags to json array
    for(auto i : source)
        data.append(i);
    return data;
}

Note::Note(const QJsonValue &val) {
    QJsonObject t = val.toObject();

    // // these are the contents when the content was not requested
    mCreateDate = getTime(t.value("createdate"));
    mIsDeleted = t.value("deleted").toInt();
    mKey = t.value("key").toString();
    mMinVersion = t.value("minversion").toInt();
    mModifyDate = getTime(t.value("modifydate").toInt());
    mSyncnum = t.value("syncnum").toInt();
    mVersion = t.value("version").toInt();
    parseJsonToVector(t.value("tags"), mTags);
    parseJsonToVector(t.value("systemtags"), mSystemTags);

    // content was requested
    mContentFetched = t.value("content") != QJsonValue::Undefined;
    if(mContentFetched)
        mContent = t.value("content").toString();

    qDebug() << "ctor note" << *this;
}

Note::Note()
{
qDebug() << "ctor note empty";
}

Note::~Note()
{
    qDebug() << "dtor " << *this;
}


QDebug operator<<(QDebug dbg, const Note &type)
{
    dbg.nospace() << "Note(" << type.getKey() << ")";
    return dbg.maybeSpace();
}
