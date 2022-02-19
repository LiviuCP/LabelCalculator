cmake_minimum_required(VERSION 3.5)

project(Settings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(Settings SHARED
    settings.cpp
    appsettings.cpp
)

target_compile_definitions(Settings PRIVATE SETTINGS_LIBRARY)
