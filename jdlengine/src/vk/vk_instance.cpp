#include "vk/vk_instance.hpp"

#include <array>
#include <cstring>
#include <vector>

#include "core/application.hpp"
#include "core/window.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

namespace
{

#ifdef NDEBUG
constexpr bool kEnableValidationLayers = false;
#else
constexpr bool kEnableValidationLayers = true;
#endif

constexpr std::array<const char*, 1> kValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

bool CheckValidationLayersSupport()
{
	uint32_t nb_layers = 0;
	VK_CHECK(vkEnumerateInstanceLayerProperties(&nb_layers, nullptr));

	std::vector<VkLayerProperties> available_layers(nb_layers);
	VK_CHECK(vkEnumerateInstanceLayerProperties(&nb_layers, VK_DATA(available_layers)));

	for (const char* layer_name : kValidationLayers)
	{
		bool found = false;
		for (const auto& properties : available_layers)
		{
			if (std::strcmp(layer_name, properties.layerName) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			JDL_ERROR("Validation layer not available: {}", layer_name);
			return false;
		}
	}
	return true;
}

const char* GetMessageTypeName(VkDebugUtilsMessageTypeFlagsEXT type)
{
	if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
		return "validation";
	}
	if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
		return "performance";
	}
	return "general";
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* /*user_data*/)
{
	const char* type_name = GetMessageTypeName(type);
	const char* message = callback_data->pMessage;

	// Never throw from this callback: it is called from within the Vulkan driver.
	if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		JDL_ERROR("Vulkan {}: {}", type_name, message);
	}
	else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		JDL_WARN("Vulkan {}: {}", type_name, message);
	}
	else {
		JDL_DEBUG("Vulkan {}: {}", type_name, message);
	}

	// The Vulkan call that triggered the message must not be aborted.
	return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT MakeDebugMessengerCreateInfo()
{
	return VkDebugUtilsMessengerCreateInfoEXT {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = DebugCallback
	};
}

} // namespace

Instance::Instance()
{
	VkApplicationInfo app_info {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = core::Application::GetName(),
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "JDLEngine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3
	};

	auto extensions = core::Window::GetInstanceExtensions();

	VkInstanceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 0
	};

	// Also chained to the instance create info, so that vkCreateInstance and
	// vkDestroyInstance calls are covered by the debug messenger.
	auto debug_create_info = MakeDebugMessengerCreateInfo();

	if constexpr (kEnableValidationLayers)
	{
		if (!CheckValidationLayersSupport()) {
			JDL_FATAL("Validation layers requested, but not available");
		}
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		create_info.pNext = &debug_create_info;
		create_info.enabledLayerCount = VK_SIZE(kValidationLayers);
		create_info.ppEnabledLayerNames = VK_DATA(kValidationLayers);
	}

	create_info.enabledExtensionCount = VK_SIZE(extensions);
	create_info.ppEnabledExtensionNames = VK_DATA(extensions);

	VK_CHECK(vkCreateInstance(&create_info, nullptr, &m_instance));

	if constexpr (kEnableValidationLayers)
	{
		auto create_messenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT")
		);
		if (create_messenger == nullptr) {
			JDL_FATAL("Failed to load vkCreateDebugUtilsMessengerEXT");
		}
		VK_CHECK(create_messenger(
			m_instance, &debug_create_info, nullptr, &m_debug_messenger
		));
		JDL_INFO("Enabled Vulkan validation layers");
	}

	JDL_INFO("Created Vulkan instance");
}

Instance::~Instance()
{
	if (m_debug_messenger != VK_NULL_HANDLE)
	{
		auto destroy_messenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT")
		);
		if (destroy_messenger != nullptr) {
			destroy_messenger(m_instance, m_debug_messenger, nullptr);
		}
	}
	vkDestroyInstance(m_instance, nullptr);
}

} // namespace vk
} // namespace jdl
