TEMPLATE = app
CONFIG += console c++11 c++14
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
        parsercreator.cpp \
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
    parsercreator.h \
    powerdevice.h
