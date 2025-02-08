# -----------------------------------------------------------------------------
# JDLEngine - Sandbox application CMakeLists.txt
# -----------------------------------------------------------------------------
set(APP_NAME sandbox)

include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)
include_directories(${VENDOR_DIR}/glfw/include)

add_executable(${APP_NAME} Main.cpp)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
