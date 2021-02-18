TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        datadevice.cpp \
        device.cpp \
        error.cpp \
        labelutils.cpp \
        main.cpp \
        powerdevice.cpp

HEADERS += \
    datadevice.h \
    device.h \
    error.h \
    labelutils.h \
    powerdevice.h
