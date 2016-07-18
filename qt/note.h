#ifndef NOTE_H
#define NOTE_H

#include <QVector>
#include <QDate>
#include <QString>

class Note
{
private:
    QVector<QString> mTags;
    QVector<QString> mSystemTags;
    QString mKey;
    QString mContent;
    QDate mCreateDate;
    QDate mModifyDate;
    bool mIsDeleted;
    int mVersion;
    int mMinVersion;
    int mSyncnum;
    bool mIsUpdated;

public:
    const QVector<QString>& getTags();
    const QVector<QString>& getSystemTags();
    const QString& getKey();
    const QString& getContent();
    const QDate& getCreateDate();
    const QDate& getModifyDate();
    bool isDeleted();
    int getVersion();
    int getMinVersion();
    int getSyncnum();
    bool isUpdated();
    void setContent(const QString& newContent);
    void setTags(const QVector<QString>& newTags);
    Note (const QString& data);
};

#endif // NOTE_H
