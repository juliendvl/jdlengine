# -------------------------------------------------------------------------------------
# JDLEngine - Main CMakeLists.txt
# -------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(JDLEngine VERSION 1.0 LANGUAGES CXX)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Use C++20 as standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Libraries: lib folder / Executables: bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(
    jdlengine
    # Entry point
    ${SRC_DIR}/main.cpp
)
