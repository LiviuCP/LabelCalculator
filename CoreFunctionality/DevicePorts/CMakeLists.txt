cmake_minimum_required(VERSION 3.5)

project(DevicePorts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    deviceports.cpp
    deviceportsfactory.cpp
    deviceport.cpp
    datadeviceport.cpp
    powerdeviceport.cpp
    deviceportutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ErrorHandling)
target_link_libraries(${PROJECT_NAME} PRIVATE CoreUtilities)
target_link_libraries(${PROJECT_NAME} PRIVATE Settings)

target_compile_definitions(${PROJECT_NAME} PRIVATE DEVICEPORTS_LIBRARY)
