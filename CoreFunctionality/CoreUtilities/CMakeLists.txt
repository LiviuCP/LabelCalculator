cmake_minimum_required(VERSION 3.5)

project(CoreUtilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../CoreData)

add_library(CoreUtilities SHARED
    coreutilities.cpp
    coreutils.cpp
)

target_compile_definitions(CoreUtilities PRIVATE COREUTILITIES_LIBRARY)
