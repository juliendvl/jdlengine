# -----------------------------------------------------------------------------
# JDLEngine - Engine CMakeLists
# -----------------------------------------------------------------------------
set(ENGINE_NAME jdlengine)

set(INC_DIR include)
set(SRC_DIR src)

file(GLOB CORE_SRC ${INC_DIR}/core/*.hpp ${SRC_DIR}/core/*.cpp)

add_definitions(-DJDL_BUILD_DLL)

add_library(
	${ENGINE_NAME} SHARED
	${CORE_SRC}
)
include_directories(${INC_DIR})
