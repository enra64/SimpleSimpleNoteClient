#-------------------------------------------------
#
# Project created by QtCreator 2016-07-10T21:45:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleNoteClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    note.cpp \
    simplenotesync.cpp \
    notelist.cpp \
    trashfilterproxymodel.cpp \
    disksync.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    note.h \
    simplenotesync.h \
    notelist.h \
    trashfilterproxymodel.h \
    disksync.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
