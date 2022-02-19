cmake_minimum_required(VERSION 3.5)

project(CoreFunctionality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Parsers)
add_subdirectory(DevicePorts)
add_subdirectory(ErrorHandling)
add_subdirectory(CoreUtilities)
