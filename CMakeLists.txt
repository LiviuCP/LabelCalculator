cmake_minimum_required(VERSION 3.5)

project(LabelCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
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

# copy application file examples into build dir
set(EXAMPLES AppIOFilesExamples)
set(SRC_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/Documentation/Examples)
set(DEST_EXAMPLES_DIR ${CMAKE_BINARY_DIR}/Examples)

add_custom_target(${EXAMPLES} ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_EXAMPLES_DIR})

add_custom_command(TARGET ${EXAMPLES} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SRC_EXAMPLES_DIR} ${DEST_EXAMPLES_DIR})
