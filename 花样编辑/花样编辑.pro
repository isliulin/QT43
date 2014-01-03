#-------------------------------------------------
#
# Project created by QtCreator 2013-12-10T15:55:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pattern
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    patternEdit/patternedit.cpp \
    patternEdit/patternworker.cpp \
    patternEdit/editdelegate.cpp \
    patternEdit/gotodialog.cpp \
    patternEdit/copydialog.cpp \
    patternEdit/pastedialog.cpp

HEADERS  += mainwindow.h \
    patternEdit/patternedit.h \
    patternEdit/patternworker.h \
    patternEdit/editdelegate.h \
    patternEdit/gotodialog.h \
    patternEdit/copydialog.h \
    patternEdit/pastedialog.h \
    patternEdit/pattern.h

FORMS    += mainwindow.ui \
    patternEdit/patternedit.ui \
    patternEdit/gotodialog.ui \
    patternEdit/copydialog.ui \
    patternEdit/pastedialog.ui
