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
    void parseJsonToVector(const QJsonValue& json, QVector<QString> &target);
    QJsonValue parseVectorToJson(const QVector<QString>& source) const;

public://getter
    const QVector<QString>& getTags();
    const QVector<QString>& getSystemTags();
    const QString& getKey() const;
    const QString& getContent() const;
    QString getHeader() const;
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
    void setModifyDate(const QDateTime& timestamp);

public://other
    QByteArray jsonDump(bool updateModificationTimestamp) const;
    bool contentHasBeenFetched() const;

    /**
     * @brief deleteKey sets the key to an empty string, so the note will be handled as a new note
     */
    void deleteKey();
    Note (const QString&);
    Note ();
    Note (const QJsonValue& val);

};

#endif // NOTE_H
