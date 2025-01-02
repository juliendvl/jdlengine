# -----------------------------------------------------------------------------
# JDLEngine - Sandbox application CMakeLists
# -----------------------------------------------------------------------------
include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)
include_directories(${VENDOR_DIR}/glfw/include)
include_directories(${VENDOR_DIR}/glad/include)
include_directories(${VENDOR_DIR}/glm)
include_directories(${VENDOR_DIR}/stb)

add_executable(sandbox Main.cpp)
target_link_libraries(sandbox jdlengine)
