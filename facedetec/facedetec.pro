#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T09:16:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = facedetec
TEMPLATE = app

INCLUDEPATH += E:\opencv\build\install\include
QMAKE_LIBDIR += E:\opencv\build\install\x86\mingw\lib

LIBS += libopencv_objdetect320.dll \
        libopencv_core320.dll

SOURCES += main.cpp\
        mainwindow.cpp \
    FaceDetec.cpp \
    FaceWorker.cpp

HEADERS  += mainwindow.h \
    FaceDetec.h \
    FaceWorker.h

FORMS    += mainwindow.ui \
    FaceDetec.ui
