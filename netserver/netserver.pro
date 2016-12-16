#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T11:21:32
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = netserver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mwworker.cpp \
    printer.cpp \
    msgproto.cpp

HEADERS  += mainwindow.h \
    mwworker.h \
    printer.h \
    msgproto.h

FORMS    += mainwindow.ui
