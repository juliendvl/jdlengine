# -----------------------------------------------------------------------------
# JDLEngine - Engine CMakeLists
# -----------------------------------------------------------------------------
set(INC_DIR ${ENGINE_DIR}/include)
set(SRC_DIR ${ENGINE_DIR}/src)

# Sources
file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)
file(GLOB UTILS_SRC ${INC_DIR}/utils/*.hpp ${SRC_DIR}/utils/*.cpp)

# DLL export definition macro (MSVC only)
if (MSVC)
	add_definitions(-DJDL_BUILD_DLL)
endif()

add_library(
	${ENGINE_NAME}
	SHARED
	${CORE_SRC}
	${UTILS_SRC}
)
include_directories(${INC_DIR})
