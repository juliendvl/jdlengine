#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "utils/logger.hpp"

#define VK_ATTR(vkType, vkName) vkType vkName = VK_NULL_HANDLE

#define VK_SIZE(vkVec) static_cast<uint32_t>(vkVec.size())
#define VK_DATA(vkVec) vkVec.data()

/**
 * @brief Evaluates a Vulkan call and raises a fatal error if it does not return
 * VK_SUCCESS. The result is checked in every build type: Vulkan errors (missing
 * driver support, out of memory...) are runtime conditions, not programming errors.
 * @param expr The Vulkan call returning a VkResult.
 */
#define VK_CHECK(expr)                                                          \
    do {                                                                        \
        const VkResult jdl_vk_result = (expr);                                  \
        if (jdl_vk_result != VK_SUCCESS) {                                      \
            JDL_FATAL("{} failed: {}", #expr, string_VkResult(jdl_vk_result));  \
        }                                                                       \
    } while (false)
