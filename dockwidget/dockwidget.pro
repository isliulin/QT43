#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:53:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = menu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    NewSession/NewSession.cpp

HEADERS  += mainwindow.h \
    NewSession/NewSession.h

FORMS    += mainwindow.ui \
    NewSession/NewSession.ui
