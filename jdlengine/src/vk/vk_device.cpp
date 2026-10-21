#include "vk/vk_device.hpp"

#include <array>
#include <cstring>

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

namespace
{

constexpr std::array<const char*, 1> kDeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool CheckDeviceExtensionsSupport(VkPhysicalDevice device)
{
	uint32_t nb_extensions = 0;
	VK_CHECK(vkEnumerateDeviceExtensionProperties(
		device, nullptr, &nb_extensions, nullptr
	));

	std::vector<VkExtensionProperties> available_extensions(nb_extensions);
	VK_CHECK(vkEnumerateDeviceExtensionProperties(
		device, nullptr, &nb_extensions, VK_DATA(available_extensions)
	));

	for (const char* extension_name : kDeviceExtensions)
	{
		bool found = false;
		for (const auto& properties : available_extensions)
		{
			if (std::strcmp(extension_name, properties.extensionName) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	return true;
}

} // namespace

Device::Device(const Instance& instance)
{
	select_physical_device(instance);
	create_device();

	JDL_INFO("Created Vulkan device ({})", device_name());
}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}

std::string Device::device_name() const
{
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &device_properties);

	return device_properties.deviceName;
}

void Device::select_physical_device(const Instance& instance)
{
	VkInstance instance_handle = instance.handle();
	VkSurfaceKHR surface = instance.surface();

	uint32_t nb_devices = 0;
	VK_CHECK(vkEnumeratePhysicalDevices(instance_handle, &nb_devices, nullptr));

	if (nb_devices == 0) {
		JDL_FATAL("Cannot find GPU with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(nb_devices);
	VK_CHECK(vkEnumeratePhysicalDevices(instance_handle, &nb_devices, VK_DATA(devices)));

	std::vector<VkPhysicalDevice> compatible_devices;
	std::vector<QueueFamilyIndices> compatible_queues;

	for (VkPhysicalDevice device : devices)
	{
		// --- Extensions and swapchain support ---
		if (!CheckDeviceExtensionsSupport(device)) {
			continue;
		}

		// --- Queue families support ---
		uint32_t nb_queues = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &nb_queues, nullptr);

		if (nb_queues == 0) {
			continue;
		}

		std::vector<VkQueueFamilyProperties> queues(nb_queues);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &nb_queues, VK_DATA(queues));

		QueueFamilyIndices queue_indices;
		for (uint32_t i = 0; i < nb_queues; ++i)
		{
			bool graphics = (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;

			VkBool32 present = VK_FALSE;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present));

			// A single family supporting both is preferred.
			if (graphics && present)
			{
				queue_indices.graphics = i;
				queue_indices.present = i;
				break;
			}
			if (graphics && queue_indices.graphics == UINT32_MAX) {
				queue_indices.graphics = i;
			}
			if (present && queue_indices.present == UINT32_MAX) {
				queue_indices.present = i;
			}
		}

		if (queue_indices.is_complete())
		{
			compatible_devices.push_back(device);
			compatible_queues.push_back(queue_indices);
		}
	}
	if (compatible_devices.empty()) {
		JDL_FATAL("Cannot find GPU matching the application requirements");
	}

	// Multiple compatible devices: use the dedicated GPU in priority.
	m_physicalDevice = compatible_devices[0];
	m_queueIndices = compatible_queues[0];

	for (size_t i = 1; i < compatible_devices.size(); ++i)
	{
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(compatible_devices[i], &device_properties);

		if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physicalDevice = compatible_devices[i];
			m_queueIndices = compatible_queues[i];

			break;
		}
	}
}

void Device::create_device()
{
	float queue_priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queue_infos;
	for (uint32_t queue_index : m_queueIndices.unique_indices())
	{
		VkDeviceQueueCreateInfo queue_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = queue_index,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
		};
		queue_infos.push_back(queue_info);
	}

	// Vulkan 1.3 device features
	VkPhysicalDeviceVulkan13Features device_features13 {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.dynamicRendering = true,
	};

	// Vulkan 1.1 device features
	VkPhysicalDeviceVulkan11Features device_features11 {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
		.pNext = &device_features13,
		.shaderDrawParameters = true,
	};

	VkPhysicalDeviceFeatures2 device_features2 {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &device_features11,
	};

	VkDeviceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &device_features2,
		.queueCreateInfoCount = VK_SIZE(queue_infos),
		.pQueueCreateInfos = VK_DATA(queue_infos),
		.enabledExtensionCount = VK_SIZE(kDeviceExtensions),
		.ppEnabledExtensionNames = VK_DATA(kDeviceExtensions)
	};
	VK_CHECK(vkCreateDevice(m_physicalDevice, &create_info, nullptr, &m_device));

	// Retrieve queue handles
	vkGetDeviceQueue(m_device, m_queueIndices.graphics, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, m_queueIndices.present, 0, &m_presentQueue);
}

} // namespace vk
} // namespace jdl
