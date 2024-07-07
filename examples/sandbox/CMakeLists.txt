# -----------------------------------------------------------------------------
# JDLEngine - Sandbox CMakeLists
# -----------------------------------------------------------------------------
set(APPLICATION_NAME Sandbox)

add_executable(${APPLICATION_NAME} Sandbox.cpp)
target_link_libraries(${APPLICATION_NAME} ${ENGINE_NAME})
