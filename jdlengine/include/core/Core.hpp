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

#include <vulkan/vulkan.h>

// --- Macros ---
#define VK_CALL(func)   {                                                               \
                            VkResult r = func;                                          \
                            if (r != VK_SUCCESS)                                        \
                                JDL_FATAL("{} failed with status {}", #func, (int)r);   \
                        }

// --- Disabled Warnings ---
#ifdef JDL_WINDOWS
    #pragma warning(disable: 4251)
#endif
