#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T17:37:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phCut
TEMPLATE = app

INCLUDEPATH += E:\opencv\build\install\include
QMAKE_LIBDIR += E:\opencv\build\install\x86\mingw\lib

LIBS += libopencv_objdetect320.dll \
        libopencv_core320.dll      \
        libopencv_imgcodecs320.dll \
        libopencv_imgproc320.dll   \

SOURCES += main.cpp\
        mainwindow.cpp \
    FaceDetec.cpp \
    OEScreen.cpp \
    PhView.cpp

HEADERS  += mainwindow.h \
    FaceDetec.h \
    OEScreen.h \
    PhView.h

FORMS    += mainwindow.ui \
    phview.ui
