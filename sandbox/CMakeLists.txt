# -----------------------------------------------------------------------------
# JDLEngine - Sandbox application CMakeLists
# -----------------------------------------------------------------------------
set(APPLICATION_NAME sandbox)

include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)
include_directories(${VENDOR_DIR}/glfw/include)
include_directories(${VENDOR_DIR}/glm)

add_executable(${APPLICATION_NAME} Main.cpp)
target_link_libraries(${APPLICATION_NAME} ${ENGINE_NAME})
