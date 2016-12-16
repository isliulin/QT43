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
    libqrencode/bitstream.c \
    libqrencode/mmask.c \
    libqrencode/mqrspec.c \
    libqrencode/qrencode.c \
    libqrencode/qrinput.c \
    libqrencode/qrspec.c \
    libqrencode/rsecc.c \
    libqrencode/split.c \
    libqrencode/mask.c \
    mwworker.cpp \
    printer.cpp \
    msgproto.cpp

HEADERS  += mainwindow.h \
    libqrencode/bitstream.h \
    libqrencode/mask.h \
    libqrencode/mmask.h \
    libqrencode/mqrspec.h \
    libqrencode/qrencode.h \
    libqrencode/qrencode_inner.h \
    libqrencode/qrinput.h \
    libqrencode/qrspec.h \
    libqrencode/rsecc.h \
    libqrencode/split.h \
    mwworker.h \
    printer.h \
    msgproto.h

FORMS    += mainwindow.ui
