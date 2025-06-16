#include "core/VulkanContext.hpp"
#include "core/Application.hpp"
#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace core
{

// --- Validation Layers ---
#ifndef NDEBUG
static const bool kValidationLayersUsed = true;
#else
static const bool kValidationLayersUsed = false;
#endif

static const std::vector<const char*> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static void s_CheckValidationLayersSupport()
{
    uint32_t nbLayers;
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, nullptr));

    std::vector<VkLayerProperties> layers(nbLayers);
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, layers.data()));

    for (const char* validationLayer : kValidationLayers)
    {
        bool layerFound = false;
        for (const auto& layer : layers)
        {
            if (strcmp(layer.layerName, validationLayer) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound)
        {
            JDL_FATAL("Validation layer {} is not supported", validationLayer);
        }
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                      VkDebugUtilsMessageTypeFlagsEXT type,
                                                      const VkDebugUtilsMessengerCallbackDataEXT* data,
                                                      void* userData)
{
    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            JDL_TRACE(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            JDL_INFO(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            JDL_WARN(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            JDL_ERROR(data->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT s_DebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = s_DebugCallback;

    return createInfo;
}

static VkResult s_CreateDebugMessenger(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void s_DestroyDebugMessenger(VkInstance instance,
                                    VkDebugUtilsMessengerEXT debugMessenger,
                                    const VkAllocationCallbacks* pAllocator)
{
    auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        fn(instance, debugMessenger, pAllocator);
    }
}


// --- VulkanContext Class ---
VulkanContext VulkanContext::CONTEXT;

void VulkanContext::doInit()
{
    if (m_instance == VK_NULL_HANDLE)
    {
        if (kValidationLayersUsed) {
            s_CheckValidationLayersSupport();
        }

        createInstance();
        createDebugMessenger();
        selectPhysicalDevice();
        createDevice();
    }
}

void VulkanContext::doDestroy()
{
    if (m_instance != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_device, nullptr);

        if (m_debugMessenger != VK_NULL_HANDLE) {
            s_DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
        }

        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}

void VulkanContext::createInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Application::GetName().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    std::vector<const char*> extensions = Window::GetRequiredInstanceExtensions();
    if (kValidationLayersUsed) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    auto debugMessengerInfo = s_DebugMessengerCreateInfo();
    if (kValidationLayersUsed)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
        createInfo.ppEnabledLayerNames = kValidationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)(&debugMessengerInfo);
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

void VulkanContext::createDebugMessenger()
{
    if (kValidationLayersUsed)
    {
        auto createInfo = s_DebugMessengerCreateInfo();
        VK_CALL(s_CreateDebugMessenger(m_instance, &createInfo, nullptr, &m_debugMessenger));
    }
}

void VulkanContext::selectPhysicalDevice()
{
    uint32_t nbDevices;

    VK_CALL(vkEnumeratePhysicalDevices(m_instance, &nbDevices, nullptr));
    if (nbDevices == 0) {
        JDL_FATAL("Cannot find any GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(nbDevices);
    VK_CALL(vkEnumeratePhysicalDevices(m_instance, &nbDevices, devices.data()));

    std::vector<VkPhysicalDevice> compatibleDevices;
    std::vector<QueueFamilyIndices> compatibleQueueFamilyIndices;
    for (VkPhysicalDevice device : devices)
    {
        QueueFamilyIndices queueFamilyIndices;

        uint32_t nbQueues;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, nullptr);

        std::vector<VkQueueFamilyProperties> queues(nbQueues);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, queues.data());

        for (auto i = 0; i < queues.size(); ++i)
        {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndices.graphics = i;
            }
            if (queueFamilyIndices.isComplete())
            {
                compatibleDevices.push_back(device);
                compatibleQueueFamilyIndices.push_back(queueFamilyIndices);

                break;
            }
        }
    }
    if (compatibleDevices.empty()) {
        JDL_FATAL("Cannot find any GPU compatible with the application requirements");
    }

    m_physicalDevice = compatibleDevices[0];
    m_queueFamilyIndices = compatibleQueueFamilyIndices[0];
    for (auto i = 1; i < compatibleDevices.size(); ++i)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(compatibleDevices[i], &properties);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            m_physicalDevice = compatibleDevices[i];
            m_queueFamilyIndices = compatibleQueueFamilyIndices[i];
        }
    }
}

void VulkanContext::createDevice()
{
    float queuePriority = 1.0f;

    // Queue creation infos
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

    // Device features
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));

    // Retrieve the queues
    vkGetDeviceQueue(m_device, *m_queueFamilyIndices.graphics, 0, &m_graphicsQueue);
}

} // namespace core
