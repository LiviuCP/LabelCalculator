TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        connectiondefinitionparser.cpp \
        connectioninput.cpp \
        datadevice.cpp \
        device.cpp \
        devicefactory.cpp \
        displayutils.cpp \
        error.cpp \
        labelutils.cpp \
        main.cpp \
        powerdevice.cpp

HEADERS += \
    connectiondefinitionparser.h \
    connectioninput.h \
    datadevice.h \
    device.h \
    devicefactory.h \
    displayutils.h \
    error.h \
    labelutils.h \
    powerdevice.h
