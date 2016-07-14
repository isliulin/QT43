#-------------------------------------------------
#
# Project created by QtCreator 2016-07-14T14:02:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thread1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login/framelogin.cpp \
    login/loginworker.cpp

HEADERS  += mainwindow.h \
    login/framelogin.h \
    login/loginworker.h


FORMS    += mainwindow.ui \
    login/framelogin.ui


