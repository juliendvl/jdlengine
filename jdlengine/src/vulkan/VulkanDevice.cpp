#include "vulkan/VulkanDevice.hpp"
#include "vulkan/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

// --- Device Extensions ---

static const std::vector<const char*> sDeviceExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static bool s_DeviceExtensionsSupported(VkPhysicalDevice device)
{
    uint32_t nbExtensions = 0;
    VK_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, nullptr));

    std::vector<VkExtensionProperties> extensions(nbExtensions);
    VK_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, extensions.data()));

    std::set<std::string> requiredExtensions(sDeviceExtensions.begin(), sDeviceExtensions.end());
    for (const auto& extension : extensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


// --- VulkanDevice Class ---

VulkanDevice::VulkanDevice()
{
    selectPhysicalDevice();
    createDevice();
    createCommandPool();
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    vkDestroyDevice(m_device, nullptr);
}

void VulkanDevice::selectPhysicalDevice()
{
    VkInstance instance = VulkanContext::GetInstance();
    VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

    uint32_t nbDevices = 0;
    VK_CALL(vkEnumeratePhysicalDevices(instance, &nbDevices, nullptr));

    if (nbDevices == 0) {
        JDL_FATAL("Cannot find GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> physicalDevices(nbDevices);
    VK_CALL(vkEnumeratePhysicalDevices(instance, &nbDevices, physicalDevices.data()));

    // Find the compatible physical devices
    std::vector<VkPhysicalDevice> compatibleDevices;
    std::vector<QueueFamilyIndices> compatibleQueueIndices;

    for (VkPhysicalDevice physicalDevice : physicalDevices)
    {
        // Check device extensions support
        if (!s_DeviceExtensionsSupported(physicalDevice)) {
            continue;
        }

        // Check queue families support
        uint32_t nbQueues = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nbQueues, nullptr);
        if (nbQueues == 0) {
            continue;
        }

        std::vector<VkQueueFamilyProperties> queues(nbQueues);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nbQueues, queues.data());

        QueueFamilyIndices queueFamilies;
        for (uint32_t i = 0; i < nbQueues; ++i)
        {
            // Graphics
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilies.graphics = i;
            }

            // Present
            VkBool32 presentSupported = VK_FALSE;
            VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupported));
            if (presentSupported) {
                queueFamilies.present = i;
            }

            if (queueFamilies.isComplete())
            {
                compatibleDevices.push_back(physicalDevice);
                compatibleQueueIndices.push_back(queueFamilies);

                break;
            }
        }
    }
    if (compatibleDevices.empty()) {
        JDL_FATAL("Cannot find GPU compatible with the application requirements");
    }

    // Multiple compatible devices: use the dedicated graphics card in priority
    m_physicalDevice = compatibleDevices[0];
    m_queueFamilyIndices = compatibleQueueIndices[0];

    for (size_t i = 1; i < compatibleDevices.size(); ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(compatibleDevices[i], &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            m_physicalDevice = compatibleDevices[0];
            m_queueFamilyIndices = compatibleQueueIndices[0];

            break;
        }
    }
}

void VulkanDevice::createDevice()
{
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (uint32_t queueIndex : m_queueFamilyIndices.getUniqueIndices())
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = sDeviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));

    // Retrieve the queue handles
    vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphics, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_queueFamilyIndices.present, 0, &m_presentQueue);
}

void VulkanDevice::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = m_queueFamilyIndices.graphics;

    VK_CALL(vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool));
}

} // namespace vk
} // namespace jdl
