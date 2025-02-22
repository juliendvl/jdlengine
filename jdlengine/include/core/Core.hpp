#pragma once


/* --- PLATFORM DETECTION --- */
#ifdef _WIN32
    #define JDL_WINDOWS
#else
    #error "Unsupported Platform!"
#endif // _WIN32


/* --- DLL IMPORT/EXPORT (WINDOWS ONLY) --- */
#ifdef JDL_WINDOWS
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif // JDL_BUILD_DLL
#else
    #define JDL_API 
#endif // JDL_WINDOWS


/* --- COMMON INCLUDES --- */
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>


/* --- MACROS --- */
#ifndef NDEBUG
    #define VK_CALL(f) { VkResult r = f; if (r != VK_SUCCESS) JDL_FATAL("{} failed with status {}", #f, (int)r); }
#else
    #define VK_CALL(f) f
#endif


/* --- PRAGMA WARNING --- */
#ifdef JDL_WINDOWS
    #pragma warning(disable: 4251)
#endif // JDL_WINDOWS
