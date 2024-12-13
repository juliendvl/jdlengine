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
	${CORE_SRC}
	${UTILS_SRC}
)
include_directories(${INC_DIR})

# spdlog
include_directories(${VENDOR_DIR}/spdlog/include)
