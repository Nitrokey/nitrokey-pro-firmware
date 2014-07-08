#-------------------------------------------------
#
# Project created by QtCreator 2012-07-30T22:18:12
#
#-------------------------------------------------

QT       += core gui 
CONFIG   += console

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
    hotpdialog.cpp \
    stick20dialog.cpp \
    stick20responsedialog.cpp \
    stick20setup.cpp \
    stick20debugdialog.cpp \
    stick20matrixpassworddialog.cpp \
    stick20hid.c \
    stick20updatedialog.cpp \
    stick20changepassworddialog.cpp \
    aboutdialog.cpp \
    stick20infodialog.cpp \
    stick20hiddenvolumedialog.cpp \
    stick20lockfirmwaredialog.cpp

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
    hotpdialog.h \
    stick20dialog.h \
    stick20responsedialog.h \
    stick20setup.h \
    stick20debugdialog.h \
    stick20matrixpassworddialog.h \
    stick20hid.h \
    stick20updatedialog.h \
    stick20changepassworddialog.h \
    aboutdialog.h \
    stick20infodialog.h \
    stick20hiddenvolumedialog.h \
    stick20lockfirmwaredialog.h

FORMS    += mainwindow.ui \
    passworddialog.ui \
    hotpdialog.ui \
    stick20dialog.ui \
    stick20responsedialog.ui \
    stick20setup.ui \
    stick20debugdialog.ui \
    stick20matrixpassworddialog.ui \
    stick20updatedialog.ui \
    stick20changepassworddialog.ui \
    aboutdialog.ui \
    stick20infodialog.ui \
    stick20hiddenvolumedialog.ui \
    stick20lockfirmwaredialog.ui

win32{
LIBS= -lsetupapi
}

macx{
LIBS=-framework IOKit -framework CoreFoundation
}

unix:!macx{
LIBS  = `pkg-config libusb-1.0 --libs` -lrt -lpthread
INCLUDEPATH += /usr/include/libusb-1.0
INCLUDEPATH += /usr/include/qt4
INCLUDEPATH += /usr/include/qt4/QtWidgets
INCLUDEPATH += /usr/include/qt4/QtGui
INCLUDEPATH += /usr/include/qt4/QtCore

}

OTHER_FILES +=

RESOURCES += \
    resources.qrc
