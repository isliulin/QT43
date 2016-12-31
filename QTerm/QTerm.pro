#-------------------------------------------------
#
# Project created by QtCreator 2016-12-31T16:02:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTerm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QTermWidget/QTermScreen.cpp \
    QTermWidget/QTermWidget.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    QTermWidget/QTermScreen.h \
    QTermWidget/QTermWidget.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui
