#-------------------------------------------------
#
# Project created by QtCreator 2017-07-15T09:53:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = move
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    PhView.cpp \
    OEScreen.cpp

HEADERS  += mainwindow.h \
    PhView.h \
    OEScreen.h

FORMS    += mainwindow.ui \
    phview.ui
