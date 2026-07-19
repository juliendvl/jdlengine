#include "vk/vulkan_device.hpp"
#include "vk/vulkan_context.hpp"
#include "vk/vulkan_instance.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

VulkanDevice::VulkanDevice()
{
	select_physical_device();
	create_device();
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

std::string VulkanDevice::get_device_name() const
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);

	return properties.deviceName;
}

void VulkanDevice::select_physical_device()
{
	VkInstance instance = VulkanContext::GetInstance().get_handle();
	VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

	uint32_t nb_devices = 0;
	VK_CALL(vkEnumeratePhysicalDevices(instance, &nb_devices, nullptr));

	if (nb_devices == 0) {
		JDL_FATAL("There are no GPU with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(nb_devices);
	VK_CALL(vkEnumeratePhysicalDevices(instance, &nb_devices, VK_DATA(devices)));

	std::vector<VkPhysicalDevice> compatible_devices;
	std::vector<QueueFamilyIndices> compatible_queues;

	for (VkPhysicalDevice device : devices)
	{
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
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				queue_indices.graphics = i;
			}

			VkBool32 present_supported = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(
				device, i, surface, &present_supported
			);
			if (present_supported) {
				queue_indices.present = i;
			}

			if (queue_indices.is_complete())
			{
				compatible_devices.push_back(device);
				compatible_queues.push_back(queue_indices);

				break;
			}
		}
		if (compatible_devices.empty()) {
			JDL_FATAL("There are no GPU meeting the application requirements");
		}
	}

	// Multiple compatible devices: use the dedicated graphics card in priority
	m_physicalDevice = compatible_devices[0];
	m_queueFamilyIndices = compatible_queues[0];

	for (auto i = 1; i < compatible_devices.size(); ++i)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(compatible_devices[i], &properties);

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physicalDevice = compatible_devices[i];
			m_queueFamilyIndices = compatible_queues[i];

			break;
		}
	}
}

void VulkanDevice::create_device()
{
	// Queues creation info
	float queue_priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queue_infos;
	for (uint32_t queue_index : m_queueFamilyIndices.get_unique_indices())
	{
		VkDeviceQueueCreateInfo queue_info {};
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.queueFamilyIndex = queue_index;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &queue_priority;

		queue_infos.push_back(queue_info);
	}

	// Vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features vulkan13_features {};
	vulkan13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	vulkan13_features.dynamicRendering = true;

	// Vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features vulkan12_features {};
	vulkan12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	vulkan12_features.pNext = &vulkan13_features;

	// Vulkan 1.1 features
	VkPhysicalDeviceVulkan11Features vulkan11_features {};
	vulkan11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	vulkan11_features.shaderDrawParameters = true;
	vulkan11_features.pNext = &vulkan12_features;

	VkPhysicalDeviceFeatures2 device_features {};
	device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	device_features.pNext = &vulkan11_features;

	VkDeviceCreateInfo create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.queueCreateInfoCount = VK_SIZE(queue_infos);
	create_info.pQueueCreateInfos = VK_DATA(queue_infos);
	create_info.enabledExtensionCount = 0;
	create_info.pNext = &device_features;

	VK_CALL(vkCreateDevice(m_physicalDevice, &create_info, nullptr, &m_device));

	vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphics, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, m_queueFamilyIndices.present, 0, &m_presentQueue);
}

} // namespace vk
} // namespace jdl
