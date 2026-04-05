# Use version 3.16 as minimum to use precompiled headers feature 
cmake_minimum_required(VERSION 3.16)
project(JDLEngine)

set(APP_NAME jdlengine)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# Use C++20 as standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(
	${APP_NAME}
	${SRC_DIR}/Main.cpp
)
