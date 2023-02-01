cmake_minimum_required(VERSION 3.5)

project(ErrorHandling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    errorhandling.cpp
    errorhandler.cpp
    error.cpp
    errortypes.cpp
    errorsettingsproxy.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Settings)

target_compile_definitions(${PROJECT_NAME} PRIVATE ERRORHANDLING_LIBRARY)
