cmake_minimum_required(VERSION 3.5)

project(LabelCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    Application
    CoreFunctionality/Parsers
    CoreFunctionality/DevicePorts
    CoreFunctionality/ErrorHandling
    Settings
)

add_subdirectory(Application)
add_subdirectory(CoreFunctionality)
add_subdirectory(Settings)

add_executable(LabelCalculator
    main.cpp
)

target_link_libraries(LabelCalculator PRIVATE Application)
target_link_libraries(LabelCalculator PRIVATE Parsers)
target_link_libraries(LabelCalculator PRIVATE Settings)
