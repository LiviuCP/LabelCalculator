cmake_minimum_required(VERSION 3.5)

project(Parsers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} STATIC
    parsers.cpp
    preparse.cpp
    parsercreator.cpp
    parser.cpp
    connectiondefinitionparser.cpp
    connectioninputparser.cpp
    parserutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE DevicePorts)
target_link_libraries(${PROJECT_NAME} PRIVATE ErrorHandling)
target_link_libraries(${PROJECT_NAME} PRIVATE CoreUtilities)

target_compile_definitions(${PROJECT_NAME} PRIVATE PARSERS_LIBRARY)
