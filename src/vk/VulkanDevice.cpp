#include "vk/VulkanDevice.hpp"
#include "vk/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

// --- DEVICE EXTENSIONS ---

static const std::vector<const char*> sDeviceExtensions {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static bool s_DeviceExtensionsSupported(VkPhysicalDevice device)
{
	uint32_t nbExtensions = 0;
	VK_CALL(vkEnumerateDeviceExtensionProperties(
		device, nullptr,
		&nbExtensions, nullptr
	));

	std::vector<VkExtensionProperties> extensions(nbExtensions);
	VK_CALL(vkEnumerateDeviceExtensionProperties(
		device, nullptr,
		&nbExtensions, extensions.data()
	));

	std::set<std::string> requiredExtensions(
		sDeviceExtensions.begin(),
		sDeviceExtensions.end()
	);

	for (const auto& extension : extensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}


// --- VulkanDevice CLASS ---

VulkanDevice::VulkanDevice()
{
	selectPhysicalDevice();
	createDevice();
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

void VulkanDevice::selectPhysicalDevice()
{
	VkInstance instance = VulkanContext::GetInstance().get();
	VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

	uint32_t nbDevices = 0;
	VK_CALL(vkEnumeratePhysicalDevices(instance, &nbDevices, nullptr));

	if (nbDevices == 0) {
		JDL_FATAL("Cannot find a GPU with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(nbDevices);
	VK_CALL(vkEnumeratePhysicalDevices(instance, &nbDevices, devices.data()));

	// Find the compatible physical devices, with their selected queue family
	// indices.
	std::vector<VkPhysicalDevice> compatibleDevices;
	std::vector<QueueFamilyIndices> compatibleQueueFamilies;

	for (VkPhysicalDevice device : devices)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		// Application requires Vulkan 1.3 at least (dynamic rendering)
		if (properties.apiVersion < VK_API_VERSION_1_3) {
			continue;
		}

		// Device extensions support
		if (!s_DeviceExtensionsSupported(device)) {
			continue;
		}

		// Queue families support
		uint32_t nbQueues = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, nullptr);

		std::vector<VkQueueFamilyProperties> queues(nbQueues);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, queues.data());

		QueueFamilyIndices queueFamilies;
		for (uint32_t i = 0; i < nbQueues; ++i)
		{
			const auto& queue = queues[i];

			if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				queueFamilies.graphics = i;
			}

			VkBool32 presentSupport;
			VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport));
			if (presentSupport) {
				queueFamilies.present = i;
			}

			if (queueFamilies.isComplete())
			{
				compatibleDevices.push_back(device);
				compatibleQueueFamilies.push_back(queueFamilies);

				break;
			}
		}
	}
	if (compatibleDevices.empty()) {
		JDL_FATAL("Cannot find a GPU matching the application requirements");
	}

	// Multiple compatible devices -> use the dedicated GPU if available
	m_physicalDevice = compatibleDevices[0];
	m_queueFamilyIndices = compatibleQueueFamilies[0];

	for (auto i = 1; i < compatibleDevices.size(); ++i)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(compatibleDevices[i], &properties);

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physicalDevice = compatibleDevices[i];
			m_queueFamilyIndices = compatibleQueueFamilies[i];

			break;
		}
	}
}

void VulkanDevice::createDevice()
{
	// Queue creation infos
	float priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	for (uint32_t queueIndex : m_queueFamilyIndices.getUniqueIndices())
	{
		VkDeviceQueueCreateInfo queueInfo {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		queueInfo.queueFamilyIndex = queueIndex;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &priority;

		queueInfos.push_back(queueInfo);
	}

	// Device featues - Extension
	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT featuresEXT {};
	featuresEXT.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
	featuresEXT.extendedDynamicState = true;

	// Device features - Vulkan 1.3
	VkPhysicalDeviceVulkan13Features features13 {};
	features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	features13.dynamicRendering = true;
	features13.pNext = &featuresEXT;

	// Device features - Vulkan 1.1
	VkPhysicalDeviceVulkan11Features features11 {};
	features11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	features11.shaderDrawParameters = true;
	features11.pNext = &features13;

	// Device features - Base
	VkPhysicalDeviceFeatures2 features {};
	features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	features.pNext = &features11;

	// Device creation
	// The features must be passed using the pNext field and not the pEnabledFeatures
	VkDeviceCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
	createInfo.pQueueCreateInfos = queueInfos.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(sDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = sDeviceExtensions.data();
	createInfo.pNext = &features;

	VK_CALL(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));

	vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphics, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, m_queueFamilyIndices.present, 0, &m_presentQueue);
}

} // namespace vk
} // namespace jdl
