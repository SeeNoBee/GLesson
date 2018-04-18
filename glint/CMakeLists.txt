cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(glint CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/../vmath/cmake/vmathExport.cmake)

option(OPTION_BUILD_GPU_TESTS "" OFF)
option(OPTION_BUILD_TESTS "" OFF)
option(OPTION_BUILD_TOOLS "" OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../glbinding glbinding)

if(NOT TARGET glbinding)
message(ERROR "glbinding not found")
endif()

file(GLOB WrapGL_SOURCES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file(GLOB WrapGL_HEADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/glint/*)

add_library(glint STATIC ${WrapGL_SOURCES})

target_include_directories(glint PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include>)

set_target_properties(glint PROPERTIES DEBUG_POSTFIX d)

target_link_libraries(glint PUBLIC glbinding vmath::vmath)

install(TARGETS glint EXPORT glint-export
ARCHIVE DESTINATION lib)

install(FILES ${WrapGL_HEADERS}
DESTINATION include/glint)

install(EXPORT glint-export
DESTINATION cmake
NAMESPACE glint::
FILE glint-export.cmake)
