# -----------------------------------------------------------------------------
# JDLEngine - Sandbox Application CMakeLists
# -----------------------------------------------------------------------------
set(APP_NAME sandbox)

include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)

add_executable(${APP_NAME} Main.cpp)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
