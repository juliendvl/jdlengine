#pragma once

#include <vulkan/vulkan.h>

#define VK_ATTR(vkType, vkName) vkType vkName = VK_NULL_HANDLE

#define VK_SIZE(vkVec) static_cast<uint32_t>(vkVec.size())
#define VK_DATA(vkVec) vkVec.data()

#ifndef NDEBUG
    #include <vulkan/vk_enum_string_helper.h>

    #include "utils/logger.hpp"

    /**
     * @brief Evaluates a Vulkan call and raises a fatal error if it does not return
     * VK_SUCCESS. The result is only checked in debug builds.
     * @param expr The Vulkan call returning a VkResult.
     */
    #define VK_CHECK(expr)                                                          \
        do {                                                                        \
            const VkResult jdl_vk_result = (expr);                                  \
            if (jdl_vk_result != VK_SUCCESS) {                                      \
                JDL_FATAL("{} failed: {}", #expr, string_VkResult(jdl_vk_result));  \
            }                                                                       \
        } while (false)
#else
    // The call is still performed, only its result is ignored.
    #define VK_CHECK(expr) static_cast<void>(expr)
#endif
