cmake_minimum_required(VERSION 3.5)

project(Parsers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../CoreData
    ../CoreUtilities
)

add_library(Parsers SHARED
    parsers.cpp
    preparse.cpp
    parsercreator.cpp
    parser.cpp
    connectiondefinitionparser.cpp
    connectioninputparser.cpp
    parserutils.cpp
)

target_link_libraries(Parsers PRIVATE DevicePorts)
target_link_libraries(Parsers PRIVATE ErrorHandling)
target_link_libraries(Parsers PRIVATE CoreUtilities)

target_compile_definitions(Parsers PRIVATE PARSERS_LIBRARY)
