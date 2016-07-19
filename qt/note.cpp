#include "note.h"

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

const QDate &Note::getCreateDate()
{
    return mCreateDate;
}

const QDate &Note::getModifyDate()
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

Note::Note(const QString &)
{

}

Note::Note()
{

}
