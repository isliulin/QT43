#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T14:02:50
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial1
TEMPLATE = app

INCLUDEPATH += Components

SOURCES += main.cpp\
        mainwindow.cpp \
    Components/escale.cpp \
    mwworker.cpp

HEADERS  += mainwindow.h \
    Components/escale.h \
    mwworker.h

FORMS    += mainwindow.ui
