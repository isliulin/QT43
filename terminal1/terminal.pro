QT += widgets serialport sql

TARGET = QTerm
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    crc16.c \
    SendSave/SendSave.cpp \
    SendSave/SSWorker.cpp \
    QTermWidget/QTermScreen.cpp \
    QTermWidget/QTermWidget.cpp \
    Modem/Modem.cpp \
    Modem/Ymodem.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    crc.h \
    SendSave/SendSave.h \
    SendSave/SSWorker.h \
    QTermWidget/QTermScreen.h \
    QTermWidget/QTermWidget.h \
    Modem/Modem.h \
    Modem/Ymodem.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    SendSave/SendSave.ui \
    Modem/modem.ui

RESOURCES += \
    terminal.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
