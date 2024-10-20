# -----------------------------------------------------------------------------
# JDLEngine - Examples CMakeLists
# -----------------------------------------------------------------------------
include_directories(${ENGINE_DIR}/include)
include_directories(${VENDOR_DIR}/spdlog/include)
include_directories(${VENDOR_DIR}/glfw/include)
include_directories(${VENDOR_DIR}/glad/include)
include_directories(${VENDOR_DIR}/glm)
include_directories(${VENDOR_DIR}/stb)
include_directories(${VENDOR_DIR}/tinygltf)

file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
foreach (CHILD ${CHILDREN})
    if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${CHILD})
        add_subdirectory(${CHILD})
    endif()
endforeach()
