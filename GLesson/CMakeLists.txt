cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(GLesson CXX)

#option(BUILD_SHARED_LIBS "" OFF)

add_subdirectory(../glint glint)

option(SFML_BUILD_AUDIO "" OFF)
option(SFML_BUILD_GRAPHICS "" OFF)
option(SFML_BUILD_NETWORK "" OFF)

add_subdirectory(../SFML SFML)

if(NOT TARGET glint)
message(ERROR "glint not found")
endif()

# if(NOT TARGET sfml-main)
# message(ERROR "SFML-main not found")
# endif()

if(NOT TARGET sfml-system)
message(ERROR "SFML-system not found")
endif()

if(NOT TARGET sfml-window)
message(ERROR "SFML-window not found")
endif()

add_executable(GLesson main.cpp)

target_link_libraries(GLesson glint sfml-system sfml-window)

target_compile_options(GLesson PRIVATE "/MP")

set(DEPENDENCIES_BIN_DIRS
    ${SFML_BINARY_DIR}/lib/Release
    ${SFML_BINARY_DIR}/lib/Debug
    ${glbinding_BINARY_DIR}/Release
    ${glbinding_BINARY_DIR}/Debug)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/environment.user.props.tmpl ${CMAKE_CURRENT_SOURCE_DIR}/environment.user.props)
set_property(TARGET GLesson PROPERTY VS_USER_PROPS ${CMAKE_CURRENT_SOURCE_DIR}/environment.user.props)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLesson)

set_target_properties(GLesson PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")