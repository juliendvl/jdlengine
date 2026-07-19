# -------------------------------------------------------------------------------------
# JDLEngine - Main CMakeLists.txt
# -------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(JDLEngine VERSION 1.0 LANGUAGES CXX)

set(APP_NAME jdlengine)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

# Use C++20 as standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Libraries: lib folder / Executables: bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Sources definition
file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)
file(GLOB UTILS_SRC ${INC_DIR}/utils/*.hpp ${SRC_DIR}/utils/*.cpp)
file(GLOB VK_SRC ${INC_DIR}/vk/*.hpp ${SRC_DIR}/vk/*.cpp)

add_executable(
    ${APP_NAME}
    # Engine sources
    ${CORE_SRC}
    ${UTILS_SRC}
    ${VK_SRC}
    # Entry point
    ${SRC_DIR}/main.cpp
)
target_include_directories(${APP_NAME} PRIVATE ${INC_DIR})

if (MSVC)
    # Necessary to compile spdlog
    target_compile_options(${APP_NAME} PRIVATE "/utf-8")
endif()

target_precompile_headers(${APP_NAME} PRIVATE ${INC_DIR}/pch.hpp)

# spdlog
add_subdirectory(${VENDOR_DIR}/spdlog)
target_include_directories(${APP_NAME} PRIVATE ${VENDOR_DIR}/spdlog/include)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(${VENDOR_DIR}/glfw)
target_include_directories(${APP_NAME} PRIVATE ${VENDOR_DIR}/glfw/include)
target_link_libraries(${APP_NAME} PRIVATE glfw)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${APP_NAME} PRIVATE Vulkan::Vulkan)
