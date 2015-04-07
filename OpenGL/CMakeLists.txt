cmake_minimum_required (VERSION 2.6)

project (OpenGL)

file (GLOB SOURCES
      "*.h"
      "*.cpp")

add_definitions(-D_UNICODE)

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable (OpenGL ${SOURCES})
target_link_libraries(OpenGL Glu32.lib OpenGL32.lib)
