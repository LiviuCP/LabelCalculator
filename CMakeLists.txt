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
    CoreFunctionality/CoreData
    CoreFunctionality/CoreUtilities
    Settings
)

# this variable is propagated to all nested libraries
set(LIB_TYPE SHARED)

# errors were encountered when building some libraries as shared on Windows so it has been decided to keep it static on this OS for the moment
if(WIN32)
    set(LIB_TYPE STATIC)
endif()

add_subdirectory(Application)
add_subdirectory(CoreFunctionality)
add_subdirectory(Settings)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Application)
target_link_libraries(${PROJECT_NAME} PRIVATE Parsers)
target_link_libraries(${PROJECT_NAME} PRIVATE Settings)
