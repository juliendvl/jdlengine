#pragma once

// --- Platform Detection ---
#ifdef _WIN32
    #define JDL_WINDOWS
#else
    #error "Unsupported Platform"
#endif

// --- DLL Import/Export ---
#ifdef JDL_WINDOWS
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif
#else
    #define JDL_API 
#endif

// --- Common Includes ---
#include <memory>
#include <string>
#include <vector>

// --- Disabled Warnings ---
#ifdef JDL_WINDOWS
    #pragma warning(disable: 4251)
#endif
