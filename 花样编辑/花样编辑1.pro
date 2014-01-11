#-------------------------------------------------
#
# Project created by QtCreator 2013-12-20T15:08:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 花样编辑1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    patternEdit/patternedit.cpp \
    patternEdit/pattimagemodel.cpp \
    patternEdit/pattimagedelegate.cpp \
    patternEdit/pattworker.cpp

HEADERS  += mainwindow.h \
    patternEdit/patternedit.h \
    patternEdit/pattimagemodel.h \
    patternEdit/pattimagedelegate.h \
    patternEdit/pattworker.h \
    patternEdit/pattern.h

FORMS    += mainwindow.ui \
    patternEdit/patternedit.ui

RESOURCES += \
    patternEdit/colorImage.qrc
