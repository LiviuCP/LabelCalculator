TEMPLATE = app
CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        application.cpp \
        connectiondefinitionparser.cpp \
        connectioninputparser.cpp \
        datadeviceport.cpp \
        deviceport.cpp \
        deviceportsfactory.cpp \
        error.cpp \
        errortypes.cpp \
        labelutils.cpp \
        main.cpp \
        parser.cpp \
        parsercreator.cpp \
        powerdeviceport.cpp

HEADERS += \
    application.h \
    connectiondefinitionparser.h \
    connectioninputparser.h \
    datadeviceport.h \
    deviceport.h \
    deviceportsfactory.h \
    error.h \
    errortypes.h \
    labelutils.h \
    parser.h \
    parsercreator.h \
    powerdeviceport.h
