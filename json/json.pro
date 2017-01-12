#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T15:45:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = json
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mwworker.cpp \
    MsgProto.cpp \
    CsIntf.cpp

HEADERS  += mainwindow.h \
    mwworker.h \
    MsgProto.h \
    CsIntf.h

FORMS    += mainwindow.ui
