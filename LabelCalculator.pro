TEMPLATE = app
CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    Application \
    Parsers \
    DevicePorts \
    ErrorHandling \
    Settings

SOURCES += \
    Application/main.cpp \
    Application/application.cpp \
    Application/applicationutils.cpp \
    Parsers/preparse.cpp \
    Parsers/parsercreator.cpp \
    Parsers/parser.cpp \
    Parsers/connectiondefinitionparser.cpp \
    Parsers/connectioninputparser.cpp \
    Parsers/parserutils.cpp \
    DevicePorts/deviceportsfactory.cpp \
    DevicePorts/deviceport.cpp \
    DevicePorts/datadeviceport.cpp \
    DevicePorts/powerdeviceport.cpp \
    ErrorHandling/errorhandler.cpp \
    ErrorHandling/error.cpp \
    ErrorHandling/errortypes.cpp \
    ErrorHandling/errorutils.cpp \
    Settings/appsettings.cpp

HEADERS += \
    Application/application.h \
    Application/applicationdata.h \
    Application/applicationutils.h \
    Parsers/preparse.h \
    Parsers/parsercreator.h \
    Parsers/parser.h \
    Parsers/connectiondefinitionparser.h \
    Parsers/connectioninputparser.h \
    Parsers/parserutils.h \
    DevicePorts/deviceportsfactory.h \
    DevicePorts/deviceport.h \
    DevicePorts/datadeviceport.h \
    DevicePorts/powerdeviceport.h \
    DevicePorts/deviceportutils.h \
    ErrorHandling/errorhandler.h \
    ErrorHandling/error.h \
    ErrorHandling/errortypes.h \
    ErrorHandling/errorutils.h \
    Settings/appsettings.h
