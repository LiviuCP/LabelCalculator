cmake_minimum_required(VERSION 3.5)

project(LabelCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    Application
    DevicePorts
    ErrorHandling
    Parsers
    Settings
    Data
    Utilities
)

add_executable(LabelCalculator
    Application/main.cpp
    Application/application.cpp
    Parsers/preparse.cpp
    Parsers/parsercreator.cpp
    Parsers/parser.cpp
    Parsers/connectiondefinitionparser.cpp
    Parsers/connectioninputparser.cpp
    DevicePorts/deviceportsfactory.cpp
    DevicePorts/deviceport.cpp
    DevicePorts/datadeviceport.cpp
    DevicePorts/powerdeviceport.cpp
    ErrorHandling/errorhandler.cpp
    ErrorHandling/error.cpp
    ErrorHandling/errortypes.cpp
    Settings/appsettings.cpp
    Utilities/applicationutils.cpp
    Utilities/parserutils.cpp
    Utilities/deviceportutils.cpp
    Utilities/errorutils.cpp
)
