# -----------------------------------------------------------------------------
# JDLEngine - Engine CMakeLists
# -----------------------------------------------------------------------------
set(ENGINE_NAME jdlengine)

set(INC_DIR include)
set(SRC_DIR src)

file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)
file(GLOB UTILS_SRC ${INC_DIR}/utils/*.hpp ${SRC_DIR}/utils/*.cpp)

add_definitions(-DJDL_BUILD_DLL)

add_library(
	${ENGINE_NAME} SHARED
	# GLAD OpenGL loader
	${VENDOR_DIR}/glad/src/glad.c
	# Engine sources
	${CORE_SRC}
	${UTILS_SRC}
)
include_directories(${INC_DIR})

# spdlog
include_directories(${VENDOR_DIR}/spdlog/include)

# GLFW
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(${VENDOR_DIR}/glfw)
include_directories(${VENDOR_DIR}/glfw/include)
target_link_libraries(jdlengine glfw)

# GLAD
include_directories(${VENDOR_DIR}/glad/include)
