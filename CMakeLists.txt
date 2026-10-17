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
    # vk module
    ${INC_DIR}/vk/vk_common.hpp
    ${INC_DIR}/vk/vk_context.hpp
    ${INC_DIR}/vk/vk_instance.hpp
    ${INC_DIR}/vk/vk_renderer.hpp
    ${SRC_DIR}/vk/vk_context.cpp
    ${SRC_DIR}/vk/vk_instance.cpp
    ${SRC_DIR}/vk/vk_renderer.cpp
)
target_include_directories(${ENGINE_NAME} PUBLIC ${INC_DIR})

# --- Precompiled headers ---
# Third-party include paths are resolved at generation time, so they can be listed here
# before the vendor targets are declared.
target_precompile_headers(
    ${ENGINE_NAME} PRIVATE
    # standard library
    <array>
    <cstdint>
    <memory>
    <optional>
    <string>
    <string_view>
    <vector>
    # third-party
    <spdlog/spdlog.h>
    <vulkan/vulkan.h>
)

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

# --- Vulkan SDK ---
find_package(Vulkan REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC Vulkan::Vulkan)
# Makes <GLFW/glfw3.h> include <vulkan/vulkan.h> and expose the Vulkan helpers
target_compile_definitions(${ENGINE_NAME} PUBLIC GLFW_INCLUDE_VULKAN)

# --- Sandbox application ---
set(APP_NAME sandbox)

add_executable(${APP_NAME} ${CMAKE_SOURCE_DIR}/sandbox/main.cpp)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
