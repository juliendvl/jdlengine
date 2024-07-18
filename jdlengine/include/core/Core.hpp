#pragma once

// --- PLATFORM DETECTION ---
#ifdef _WIN32
    #define JDL_WINDOWS
#else
    #error "Unsupported platform!"
#endif

// --- DLL IMPORT/EXPORT ---
#ifdef JDL_WINDOWS
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif
#else
    #define JDL_API 
#endif

// --- COMMON INCLUDES ---
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "glad/glad.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>

// --- WARNING DISABLING ---
#pragma warning(disable: 4251)
