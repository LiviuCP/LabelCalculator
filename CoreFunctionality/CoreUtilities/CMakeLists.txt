cmake_minimum_required(VERSION 3.5)

project(CoreUtilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    coreutilities.cpp
    coreutils.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE COREUTILITIES_LIBRARY)
