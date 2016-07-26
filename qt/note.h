#ifndef NOTE_H
#define NOTE_H

#include <QVector>
#include <QDateTime>
#include <QString>
#include <QDebug>

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
    bool isDeleted() const;
    int getVersion() const;
    int getMinVersion() const;
    int getSyncnum() const;
    bool isNewNote() const;

    /**
     * @brief parseJsonToNotelist Parse JSON data (in simplenote format) into a vector of notes
     * @param json raw JSON data received
     * @return NoteList with the parsed json data
     */
    static QVector<Note*>* parseJsonToNotelist(const QJsonObject& json);

public://setter
    void setContent(const QString& newContent);
    void setTags(const QVector<QString>&);
    void setModifyDate(const QDateTime& timestamp);
    void setDeleted(bool del);

public://other
    QJsonDocument jsonDump(bool updateModificationTimestamp) const;
    bool contentHasBeenFetched() const;

    /**
     * @brief deleteKey sets the key to an empty string, so the note will be handled as a new note
     */
    void deleteKey();
    Note (const QJsonValue& val);
    Note();
    ~Note();
};

QDebug operator<<(QDebug dbg, const Note &type);

#endif // NOTE_H
