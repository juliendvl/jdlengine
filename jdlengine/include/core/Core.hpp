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

// --- WARNING DISABLING ---
#pragma warning(disable: 4251)
