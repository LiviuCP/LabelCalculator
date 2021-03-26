TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        connectiondefinitionparser.cpp \
        connectioninputparser.cpp \
        datadevice.cpp \
        device.cpp \
        devicefactory.cpp \
        displayutils.cpp \
        error.cpp \
        labelutils.cpp \
        main.cpp \
        parser.cpp \
        powerdevice.cpp

HEADERS += \
    connectiondefinitionparser.h \
    connectioninputparser.h \
    datadevice.h \
    device.h \
    devicefactory.h \
    displayutils.h \
    error.h \
    labelutils.h \
    parser.h \
    powerdevice.h
