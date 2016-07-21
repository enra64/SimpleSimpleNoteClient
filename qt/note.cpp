#include "note.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

const QVector<QString> &Note::getTags()
{
    return mTags;
}

const QVector<QString> &Note::getSystemTags()
{
    return mSystemTags;
}

const QString &Note::getKey() const
{
    return mKey;
}

QString Note::getContent()
{
    return mContent;
}

const QString &Note::getContent() const
{
    return mContent;
}

const QDateTime &Note::getCreateDate()
{
    return mCreateDate;
}

const QDateTime &Note::getModifyDate()
{
    return mModifyDate;
}

bool Note::isDeleted()
{
    return mIsDeleted;
}

int Note::getVersion()
{
    return mVersion;
}

int Note::getMinVersion()
{
    return mMinVersion;
}

int Note::getSyncnum()
{
    return mSyncnum;
}

bool Note::isUpdated()
{
    return false;
}

void Note::setTags(const QVector<QString> &)
{

}

bool Note::contentHasBeenFetched() const
{
    return mContentFetched;
}

Note::Note(const QString &)
{

}

Note::Note()
{

}

QDateTime Note::getTime(const QJsonValue &from)
{
    // get data as string
    QString raw = from.toString();
    // truncate to integer part of time
    QString integerPart = raw.remove(raw.indexOf('.'), raw.length());
    // parse & return
    return QDateTime::fromTime_t(integerPart.toInt());
}

Note::Note(const QJsonValue &val)
{
    QJsonObject t = val.toObject();

    // these are the contents without calling getNote
    mCreateDate = getTime(t.value("createdate"));
    mIsDeleted = t.value("deleted").toBool();
    mKey = t.value("key").toString();
    mMinVersion = t.value("minversion").toInt();
    mModifyDate = getTime(t.value("modifydate").toInt());
    mSyncnum = t.value("syncnum").toInt();
    //systags & tags missing
    mVersion = t.value("version").toInt();

    // these are the contents with calling getNote
    mContentFetched = t.value("content") != QJsonValue::Undefined;
    if(mContentFetched)
        mContent = t.value("content").toString();
}
