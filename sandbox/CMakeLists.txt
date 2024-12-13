# -----------------------------------------------------------------------------
# JDLEngine - Sandbox application CMakeLists
# -----------------------------------------------------------------------------
include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)

add_executable(sandbox Main.cpp)
target_link_libraries(sandbox jdlengine)
