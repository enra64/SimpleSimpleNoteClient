#include "disksync.h"
#include "note.h"

#include <QVector>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QSettings>

#include <assert.h>

void DiskSync::saveToDisk(const QVector<Note> &noteList)
{
    // output array
    QJsonArray data;

    // add all notes to the array
    for(const Note& n : noteList)
        data.append(n.jsonDump(false).object());

    QJsonObject wrapper;
    wrapper.insert("count", QJsonValue(noteList.length()));
    wrapper.insert("data", data);

    // save into global settings storage
    QSettings settings;

    // qjsonarrays are directly saveable
    settings.setValue("data", QJsonDocument(wrapper).toJson());
}

QVector<Note*>* DiskSync::readFromDisk()
{
    // qsettings access
    QSettings settings;

    // load from global settings storage
    //QJsonDocument obj = QJsonDocument::fromVariant(settings.value("data"));

    QByteArray var = settings.value("data").toByteArray();


    // return list of notes
    return Note::parseJsonToNotelist(QJsonDocument::fromJson(var).object());
}
