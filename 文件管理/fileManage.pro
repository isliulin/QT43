#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T14:52:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileManage
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileManage/filemanage.cpp \
    fileManage/fileworker.cpp

HEADERS  += mainwindow.h \
    fileManage/filemanage.h \
    fileManage/fileworker.h

FORMS    += mainwindow.ui \
    fileManage/filemanage.ui
