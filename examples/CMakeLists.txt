# -----------------------------------------------------------------------------
# JDLEngine - Examples CMakeLists
# -----------------------------------------------------------------------------
include_directories(${ENGINE_DIR}/include)

file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
foreach (CHILD ${CHILDREN})
    if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${CHILD})
        add_subdirectory(${CHILD})
    endif()
endforeach()
