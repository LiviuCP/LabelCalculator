TEMPLATE = app
CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    Parsers \
    DevicePorts \
    ErrorHandling \
    Settings \
    Utils

SOURCES += \
    Application/main.cpp \
    Application/application.cpp \
    Parsers/parsercreator.cpp \
    Parsers/parser.cpp \
    Parsers/connectiondefinitionparser.cpp \
    Parsers/connectioninputparser.cpp \
    DevicePorts/deviceportsfactory.cpp \
    DevicePorts/deviceport.cpp \
    DevicePorts/datadeviceport.cpp \
    DevicePorts/powerdeviceport.cpp \
    ErrorHandling/error.cpp \
    ErrorHandling/errortypes.cpp \
    Settings/appsettings.cpp \
    Utils/labelutils.cpp

HEADERS += \
    Application/application.h \
    Parsers/parsercreator.h \
    Parsers/parser.h \
    Parsers/connectiondefinitionparser.h \
    Parsers/connectioninputparser.h \
    DevicePorts/deviceportsfactory.h \
    DevicePorts/deviceport.h \
    DevicePorts/datadeviceport.h \
    DevicePorts/powerdeviceport.h \
    ErrorHandling/error.h \
    ErrorHandling/errortypes.h \
    Settings/appsettings.h \
    Utils/labelutils.h
