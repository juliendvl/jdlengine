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

# Source files
file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)
file(GLOB UTILS_SRC ${INC_DIR}/utils/*.hpp ${SRC_DIR}/utils/*.cpp)

# Necessary to compile spdlog with MSVC
if (MSVC)
	add_compile_options("/utf-8")
endif()

add_executable(
	${APP_NAME}
	${SRC_DIR}/Main.cpp
	${CORE_SRC}
	${UTILS_SRC}
)
target_include_directories(${APP_NAME} PUBLIC ${INC_DIR})

# Setup pch.hpp as precompiled header
target_precompile_headers(${APP_NAME} PRIVATE ${INC_DIR}/pch.hpp)

# spdlog
target_include_directories(${APP_NAME} PUBLIC ${VENDOR_DIR}/spdlog/include)
