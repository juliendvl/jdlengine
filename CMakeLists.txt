# --------------------------------------------------------------------------------------
# JDLEngine - Main CMakeLists
# --------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(JDLEngine LANGUAGES CXX)

# --- Toplevel variables ---
set(ENGINE_NAME jdlengine)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/jdlengine)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# --- Use C++20 as standard ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Set output directories ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Header/Source directories ---
set(INC_DIR ${ENGINE_DIR}/include)
set(SRC_DIR ${ENGINE_DIR}/src)

add_library(
    ${ENGINE_NAME} STATIC
    # core module
    ${INC_DIR}/core/application.hpp
    ${SRC_DIR}/core/application.cpp
    # utils module
    ${INC_DIR}/utils/non_copyable.hpp
)
target_include_directories(${ENGINE_NAME} PUBLIC ${INC_DIR})

# --- Sandbox application ---
set(APP_NAME sandbox)

add_executable(${APP_NAME} ${CMAKE_SOURCE_DIR}/sandbox/main.cpp)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
