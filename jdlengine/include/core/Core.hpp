#pragma once


// Platform Detection
#ifdef _WIN32
    #define JDL_WINDOWS
#else
    #error "Unsupported Platform"
#endif


// DLL Import/Export (Windows only)
#ifdef JDL_WINDOWS
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif
#else
    #define JDL_API 
#endif


// Common Includes
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>


// Warnings
#pragma warning(disable : 4251)
