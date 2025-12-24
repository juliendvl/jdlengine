# -----------------------------------------------------------------------------
# jdlengine - Engine CMakeLists
# -----------------------------------------------------------------------------
set(INC_DIR ${ENGINE_DIR}/include)
set(SRC_DIR ${ENGINE_DIR}/src)

file(GLOB CORE_SRC     ${INC_DIR}/core/*.hpp     ${SRC_DIR}/core/*.cpp)
file(GLOB MATH_SRC     ${INC_DIR}/math/*.hpp     ${SRC_DIR}/math/*.cpp)
file(GLOB RESOURCE_SRC ${INC_DIR}/resource/*.hpp ${SRC_DIR}/resource/*.cpp)
file(GLOB UTILS_SRC    ${INC_DIR}/utils/*.hpp    ${SRC_DIR}/utils/*.cpp)
file(GLOB VULKAN_SRC   ${INC_DIR}/vulkan/*.hpp   ${SRC_DIR}/vulkan/*.cpp)

# DLL Export specification macro
if (MSVC)
	add_definitions(-DJDL_BUILD_DLL)
endif()

add_library(
	${ENGINE_NAME}
	SHARED
	${CORE_SRC}
	${MATH_SRC}
	${RESOURCE_SRC}
	${UTILS_SRC}
	${VULKAN_SRC}
)
include_directories(${INC_DIR})
target_precompile_headers(${ENGINE_NAME} PRIVATE ${INC_DIR}/core/Core.hpp)

# spdlog
include_directories(${VENDOR_DIR}/spdlog/include)

# glfw
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(${VENDOR_DIR}/glfw)
include_directories(${VENDOR_DIR}/glfw/include)
target_link_libraries(${ENGINE_NAME} glfw)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${ENGINE_NAME} Vulkan::Vulkan)

# glm
include_directories(${VENDOR_DIR}/glm)
