# -----------------------------------------------------------------------------
# jdlengine - Main CMakeLists
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.11)
project(jdlengine)

set(ENGINE_NAME jdlengine)
set(ENGINE_DIR  ${CMAKE_SOURCE_DIR}/${ENGINE_NAME})
set(VENDOR_DIR  ${ENGINE_DIR}/vendor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Use C++17 as standard
set(CMAKE_CXX_STANDARD 17)
# Setup compile options
if (MSVC)
	set(CMAKE_CXX_FLAGS "/utf-8 ${CMAKE_CXX_FLAGS}")
endif()

# Engine library
add_subdirectory(${ENGINE_DIR})
# sandbox application
option(BUILD_SANDBOX "Build the sandbox application" ON)

if (BUILD_SANDBOX)
	add_subdirectory(${CMAKE_SOURCE_DIR}/sandbox)
endif()
