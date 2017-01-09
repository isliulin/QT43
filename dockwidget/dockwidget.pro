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
    NewSession/NewSession.cpp \
    NewSession/SerialSetting.cpp \
    NewSession/TelnetSetting.cpp \
    NewSession/Setting.cpp \
    NewSession/TftpsSetting.cpp \
    SerialAssistant/SerialAssistant.cpp

HEADERS  += mainwindow.h \
    NewSession/NewSession.h \
    NewSession/SerialSetting.h \
    NewSession/TelnetSetting.h \
    NewSession/Setting.h \
    NewSession/TftpsSetting.h \
    SerialAssistant/SerialAssistant.h

FORMS    += mainwindow.ui \
    NewSession/NewSession.ui \
    NewSession/SerialSetting.ui \
    NewSession/TelnetSetting.ui \
    NewSession/TftpsSetting.ui \
    SerialAssistant/SerialAssistant.ui
