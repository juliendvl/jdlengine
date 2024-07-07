# -----------------------------------------------------------------------------
# JDLEngine - Engine CMakeLists
# -----------------------------------------------------------------------------
set(INC_DIR ${ENGINE_DIR}/include)
set(SRC_DIR ${ENGINE_DIR}/src)

file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)
file(GLOB UTILS_SRC ${INC_DIR}/utils/*.hpp ${SRC_DIR}/utils/*.cpp)

if (MSVC)
	add_definitions(-DJDL_BUILD_DLL)
endif()

add_library(
	${ENGINE_NAME}
	SHARED
	# glad
	${VENDOR_DIR}/glad/src/glad.c
	# jdlengine
	${CORE_SRC}
	${UTILS_SRC}
)
include_directories(${INC_DIR})

# spdlog
include_directories(${VENDOR_DIR}/spdlog/include)

# glfw
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(${VENDOR_DIR}/glfw)
include_directories(${VENDOR_DIR}/glfw/include)
target_link_libraries(${ENGINE_NAME} glfw)

# glad
include_directories(${VENDOR_DIR}/glad/include)
