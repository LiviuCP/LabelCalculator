cmake_minimum_required(VERSION 3.5)

project(Application LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} STATIC
    application.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE APPLICATION_LIBRARY)
