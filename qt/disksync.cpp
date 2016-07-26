#include "disksync.h"
#include "note.h"

#include <QVector>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>

#include <assert.h>

DiskSync::DiskSync(const QString &path) : mPath(path){

}

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

    // create output file
    QFile file(mPath);

    if(file.open(QIODevice::WriteOnly)){
        file.write(QJsonDocument(wrapper).toJson());
    }
    else
        assert(false && "could not open file");

}

QVector<Note*>* DiskSync::readFromDisk()
{
    // create file handle
    QFile inFile(mPath);

    // open file
    inFile.open(QIODevice::ReadOnly);

    // parse json from file
    QString dataS = inFile.readAll();
    qDebug() << dataS;
    QJsonObject data = QJsonDocument::fromJson(dataS.toUtf8()).object();

    // return list of notes
    return Note::parseJsonToNotelist(data);
}
