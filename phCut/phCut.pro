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

LIBS += E:\opencv\build\install\x86\mingw\bin\libopencv_objdetect320.dll \
        E:\opencv\build\install\x86\mingw\bin\libopencv_core320.dll      \
        E:\opencv\build\install\x86\mingw\bin\libopencv_imgcodecs320.dll \
        E:\opencv\build\install\x86\mingw\bin\libopencv_imgproc320.dll   \

SOURCES += main.cpp\
        mainwindow.cpp \
    FaceDetec.cpp \
    OEScreen.cpp \
    PhView.cpp \
    PhFinder.cpp

HEADERS  += mainwindow.h \
    FaceDetec.h \
    OEScreen.h \
    PhView.h \
    PhFinder.h

FORMS    += mainwindow.ui \
    phview.ui
