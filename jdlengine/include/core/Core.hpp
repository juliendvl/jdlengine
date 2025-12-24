#pragma once


// --- DLL Import/Export ---
#ifdef _WIN32
    #ifdef JDL_BUILD_DLL
        #define JDL_API __declspec(dllexport)
    #else
        #define JDL_API __declspec(dllimport)
    #endif // JDL_BUILD_DLL
#else
    #define JDL_API 
#endif // _WIN32


// --- Common Includes ---
#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

// --- Macros ---
#ifndef NDEBUG
    #define VK_CALL(f)  {                                                           \
                            VkResult r = f;                                         \
                            if (r != VK_SUCCESS)                                    \
                                JDL_FATAL("{} failed with status {}", #f, (int)r);  \
                        }
#else
    #define VK_CALL(f) f
#endif // !NDEBUG


// --- Pragma Warnings ---
#pragma warning(disable: 4251)
