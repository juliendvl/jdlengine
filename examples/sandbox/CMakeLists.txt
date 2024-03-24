# -----------------------------------------------------------------------------
# JDLEngine - Sandbox application CMakeLists
# -----------------------------------------------------------------------------
set(APP_NAME Sandbox)

add_executable(${APP_NAME} Sandbox.cpp)
target_link_libraries(${APP_NAME} jdlengine)
