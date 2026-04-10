#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>


#define VK_ATTR(vkType, attrName) vkType attrName = VK_NULL_HANDLE

#ifndef NDEBUG
	#define VK_CALL(vkFunc) {																\
								VkResult r = vkFunc;										\
								if (r != VK_SUCCESS)										\
									JDL_FATAL("{} failed with status", #vkFunc, (int)r);	\
							}
#else
	#define VK_CALL(vkFunc) vkFunc
#endif
