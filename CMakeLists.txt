# --------------------------------------------------------------------------------------
# JDLEngine - Main CMakeLists
# --------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(JDLEngine LANGUAGES CXX)

# --- Toplevel variables ---
set(ENGINE_NAME jdlengine)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/jdlengine)
set(VENDOR_DIR ${ENGINE_DIR}/vendor)

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
    ${INC_DIR}/core/size.hpp
    ${INC_DIR}/core/window.hpp
    ${SRC_DIR}/core/application.cpp
    ${SRC_DIR}/core/window.cpp
    # utils module
    ${INC_DIR}/utils/logger.hpp
    ${INC_DIR}/utils/non_copyable.hpp
    ${SRC_DIR}/utils/logger.cpp
)
target_include_directories(${ENGINE_NAME} PUBLIC ${INC_DIR})

# --- Vendor: spdlog ---
add_subdirectory(${VENDOR_DIR}/spdlog)
target_link_libraries(${ENGINE_NAME} PUBLIC spdlog::spdlog)

# --- Vendor: glfw ---
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${VENDOR_DIR}/glfw)

target_include_directories(
    ${ENGINE_NAME} PUBLIC
    ${VENDOR_DIR}/glfw/include
)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

# --- Sandbox application ---
set(APP_NAME sandbox)

add_executable(${APP_NAME} ${CMAKE_SOURCE_DIR}/sandbox/main.cpp)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
