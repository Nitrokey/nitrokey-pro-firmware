#-------------------------------------------------
#
# Project created by QtCreator 2012-07-30T22:18:12
#
#-------------------------------------------------

QT       += core gui

TARGET = CryptoStickGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hotpslot.cpp \
    device.cpp \
    response.cpp \
    command.cpp \
    crc32.cpp \
    totpslot.cpp \
    base32.cpp \
    passworddialog.cpp \
    hotpdialog.cpp

win32 {

 SOURCES +=   hid_win.c
}

macx{
 SOURCES +=   hid_mac.c
}

unix:!macx{
  SOURCES +=   hid_libusb.c
}

HEADERS  += mainwindow.h \
    hidapi.h \
    hotpslot.h \
    device.h \
    response.h \
    command.h \
    crc32.h \
    sleep.h \
    totpslot.h \
    base32.h \
    passworddialog.h \
    hotpdialog.h

FORMS    += mainwindow.ui \
    passworddialog.ui \
    hotpdialog.ui

win32{
LIBS= -lsetupapi
}

macx{
LIBS=-framework IOKit -framework CoreFoundation
}

unix{
LIBS  = `pkg-config libusb-1.0 --libs` -lrt -lpthread
INCLUDEPATH += /usr/include/libusb-1.0
}

OTHER_FILES +=

RESOURCES += \
    resources.qrc
