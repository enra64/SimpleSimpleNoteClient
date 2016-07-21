#ifndef NOTE_H
#define NOTE_H

#include <QVector>
#include <QDateTime>
#include <QString>

class QJsonValue;

class Note
{
private://note contents
    QVector<QString> mTags;
    QVector<QString> mSystemTags;
    QString mKey;
    QString mContent;
    QDateTime mCreateDate;
    QDateTime mModifyDate;
    bool mIsDeleted;
    int mVersion;
    int mMinVersion;
    int mSyncnum;
    bool mIsUpdated;

private://custom members & functions
    bool mContentFetched;
    QDateTime getTime(const QJsonValue& from);

public://getter
    const QVector<QString>& getTags();
    const QVector<QString>& getSystemTags();
    const QString& getKey() const;
    const QString& getContent() const;
    QString getContent();
    const QDateTime& getCreateDate();
    const QDateTime& getModifyDate();
    bool isDeleted();
    int getVersion();
    int getMinVersion();
    int getSyncnum();
    bool isUpdated();

public://setter
    void setContent(const QString& newContent);
    void setTags(const QVector<QString>&);

public://other
    bool contentHasBeenFetched() const;
    Note (const QString&);
    Note ();
    Note (const QJsonValue& val);
};

#endif // NOTE_H
