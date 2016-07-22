#include "note.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

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

bool Note::isDeleted() {
    return mIsDeleted;
}

int Note::getVersion() {
    return mVersion;
}

int Note::getMinVersion() {
    return mMinVersion;
}

int Note::getSyncnum() {
    return mSyncnum;
}

bool Note::isUpdated() {
    return false;
}

void Note::setContent(const QString &newContent) {
    mContent = newContent;
}

void Note::setTags(const QVector<QString> &) {

}

void Note::setModifyDate(const QDateTime &timestamp) {
    mModifyDate = timestamp;
}

QByteArray Note::jsonDump() const {
    QJsonObject dump;
    dump.insert("deleted", QJsonValue(mIsDeleted));
    dump.insert("createdate", QJsonValue(mCreateDate.toString()));
    dump.insert("content", QJsonValue(mContent));
    dump.insert("version", QJsonValue(mVersion));
    dump.insert("systemtags", parseVectorToJson(mSystemTags));
    dump.insert("key", QJsonValue(mKey));
    dump.insert("tags", parseVectorToJson(mTags));
    dump.insert("minversion", QJsonValue(mMinVersion));
    dump.insert("modifydate", QJsonValue(mModifyDate.toString()));
    dump.insert("syncnum", QJsonValue(mSyncnum));
    return QJsonDocument(dump).toBinaryData();
}

bool Note::contentHasBeenFetched() const {
    return mContentFetched;
}

Note::Note(const QString &) {

}

Note::Note() {

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

    // these are the contents without calling getNote
    mCreateDate = getTime(t.value("createdate"));
    mIsDeleted = t.value("deleted").toBool();
    mKey = t.value("key").toString();
    mMinVersion = t.value("minversion").toInt();
    mModifyDate = getTime(t.value("modifydate").toInt());
    mSyncnum = t.value("syncnum").toInt();
    mVersion = t.value("version").toInt();
    parseJsonToVector(t.value("tags"), mTags);
    parseJsonToVector(t.value("systemtags"), mSystemTags);

    // these are the contents with calling getNote
    mContentFetched = t.value("content") != QJsonValue::Undefined;
    if(mContentFetched)
        mContent = t.value("content").toString();
}
