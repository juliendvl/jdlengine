# -----------------------------------------------------------------------------
# JDLEngine - Main CMakeLists.txt
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.11)
project(JDLEngine)

set(ENGINE_NAME jdlengine)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/jdlengine)
set(VENDOR_DIR ${ENGINE_DIR}/vendor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# Engine library subdirectory
add_subdirectory(${ENGINE_DIR})
# Sandbox Application subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/sandbox)
