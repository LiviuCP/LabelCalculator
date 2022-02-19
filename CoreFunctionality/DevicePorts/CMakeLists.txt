cmake_minimum_required(VERSION 3.5)

project(DevicePorts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../CoreUtilities
    ../CoreData
)

add_library(DevicePorts SHARED
    deviceports.cpp
    deviceportsfactory.cpp
    deviceport.cpp
    datadeviceport.cpp
    powerdeviceport.cpp
    deviceportutils.cpp
)

target_link_libraries(DevicePorts PRIVATE ErrorHandling)
target_link_libraries(DevicePorts PRIVATE CoreUtilities)
target_link_libraries(DevicePorts PRIVATE Settings)

target_compile_definitions(DevicePorts PRIVATE DEVICEPORTS_LIBRARY)
