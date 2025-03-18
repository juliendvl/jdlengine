#include "core/VulkanContext.hpp"
#include "core/Application.hpp"
#include "core/Window.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Shader.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

/* --- VALIDATION LAYERS --- */

static const std::vector<const char*> sValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifndef NEDBUG
static const bool sUseValidationLayers = true;
#else
static const bool sUseValidationLayers = false;
#endif

// Checks whether the required validation layers are supported or not
static bool s_CheckValidationLayersSupport()
{
    uint32_t nbLayers;
    vkEnumerateInstanceLayerProperties(&nbLayers, nullptr);

    std::vector<VkLayerProperties> layers(nbLayers);
    vkEnumerateInstanceLayerProperties(&nbLayers, layers.data());

    for (const char* requiredLayer : sValidationLayers)
    {
        bool found = false;
        for (const auto& layer : layers)
        {
            if (strcmp(layer.layerName, requiredLayer) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            JDL_FATAL("Validation layer {} is not supported", requiredLayer);
        }
    }

    return true;
}

// Debug callback
static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                      void* pUserData)
{
    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            JDL_TRACE(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            JDL_INFO(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            JDL_WARN(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            JDL_ERROR(pCallbackData->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}

// Creates and returns the debug messenger info creation structure
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
    createInfo.pUserData = nullptr;

    return createInfo;
}

// Creates the debug messenger object
static VkResult s_CreateDebugMessenger(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (fn == nullptr)
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
    return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

// Destroys the debug messenger object
static void s_DestroyDebugMessenger(VkInstance instance,
                                    VkDebugUtilsMessengerEXT debugMessenger,
                                    const VkAllocationCallbacks* pAllocator)
{
    auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (fn != nullptr)
    {
        fn(instance, debugMessenger, pAllocator);
    }
}


/* --- DEVICE EXTENSIONS --- */

static const std::vector<const char*> sDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Checks whether the physical device supports the required device extensions or not
static bool s_SupportsDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t nbExtensions;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, nullptr);

    std::vector<VkExtensionProperties> extensions(nbExtensions);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, extensions.data());

    std::set<std::string> requiredExtensions(sDeviceExtensions.begin(), sDeviceExtensions.end());
    for (const auto& extension : extensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}


/* --- VulkanContext CLASS --- */

VulkanContext VulkanContext::IContext;

VulkanContext::VulkanContext()
    : m_instance(VK_NULL_HANDLE)
    , m_debugMessenger(VK_NULL_HANDLE)
    , m_surface(VK_NULL_HANDLE)
    , m_physicalDevice(VK_NULL_HANDLE)
    , m_device(VK_NULL_HANDLE)
{}

void VulkanContext::RecreateSwapChain()
{
    VK_CALL(vkDeviceWaitIdle(IContext.m_device));

    // Delete the old swap chain
    IContext.m_swapChain.reset();
    // Recreate the swap chain
    IContext.m_swapChain = std::make_unique<SwapChain>();
    IContext.m_swapChain->createFramebuffers();
}

void VulkanContext::init()
{
    if (m_instance == VK_NULL_HANDLE)
    {
        createInstance();
        createDebugMessenger();
        createWindowSurface();
        selectPhysicalDevice();
        createDevice();
        setupQueues();
        createDefaultResources();
        createSwapChain();
        createPipeline();
    }
}

void VulkanContext::destroy()
{
    if (m_instance != VK_NULL_HANDLE)
    {
        m_pipeline.reset();
        m_swapChain.reset();

        for (const auto& [index, queue] : m_queues)
        {
            vkDestroyCommandPool(m_device, queue.commandPool, nullptr);
        }

        vkDestroyDevice(m_device, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        if (m_debugMessenger != VK_NULL_HANDLE)
        {
            s_DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
        }
        vkDestroyInstance(m_instance, nullptr);
    }
    m_instance = VK_NULL_HANDLE;
}

void VulkanContext::createInstance()
{
    auto debugMessengerCreateInfo = s_DebugMessengerCreateInfo();
    if (sUseValidationLayers)
    {
        // Check that the required validation layers are supported
        s_CheckValidationLayersSupport();
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Application::GetName();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Required instance extensions
    std::vector<const char*> extensions = Window::GetInstanceExtensions();
    if (sUseValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (sUseValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(sValidationLayers.size());
        createInfo.ppEnabledLayerNames = sValidationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

void VulkanContext::createDebugMessenger()
{
    if (sUseValidationLayers)
    {
        auto createInfo = s_DebugMessengerCreateInfo();
        VK_CALL(s_CreateDebugMessenger(m_instance, &createInfo, nullptr, &m_debugMessenger));
    }
}

void VulkanContext::createWindowSurface()
{
    m_surface = Application::GetWindow().createWindowSurface(m_instance);
}

void VulkanContext::selectPhysicalDevice()
{
    uint32_t nbDevices;
    vkEnumeratePhysicalDevices(m_instance, &nbDevices, nullptr);
    if (nbDevices == 0)
    {
        JDL_FATAL("Cannot find a physical device with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(nbDevices);
    vkEnumeratePhysicalDevices(m_instance, &nbDevices, devices.data());

    // Find the compatible physical devices
    std::vector<VkPhysicalDevice> validDevices;
    std::vector<QueueFamilyIndices> validQueueFamilies;

    for (VkPhysicalDevice device : devices)
    {
        // Device extension support
        if (!s_SupportsDeviceExtensions(device))
        {
            continue;
        }

        QueueFamilyIndices queueFamilies;

        uint32_t nbQueues;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, nullptr);

        std::vector<VkQueueFamilyProperties> queues(nbQueues);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, queues.data());

        for (uint32_t i = 0; i < nbQueues; ++i)
        {
            VkQueueFamilyProperties queueProperties = queues[i];

            // Graphics queue family test
            if (queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamilies.graphics = i;
            }

            // Presentation queue family test
            VkBool32 presentSupported = VK_FALSE;
            VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupported));
            if (presentSupported)
            {
                queueFamilies.present = i;
            }

            if (queueFamilies.isComplete())
            {
                validDevices.push_back(device);
                validQueueFamilies.push_back(queueFamilies);
                break;
            }
        }
    }
    if (validDevices.empty())
    {
        JDL_FATAL("Cannot find a physical device compatible with the application");
    }

    // Multiple compatible devices -> use the dedicated GPU if existing
    m_physicalDevice = validDevices[0];
    m_queueFamilyIndices = validQueueFamilies[0];

    for (auto i = 1; i < validDevices.size(); ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(validDevices[i], &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            m_physicalDevice = validDevices[i];
            m_queueFamilyIndices = validQueueFamilies[i];
            break;
        }
    }
}

void VulkanContext::createDevice()
{
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
    for (uint32_t queueIndex : m_queueFamilyIndices.getUniqueIndices())
    {
        VkDeviceQueueCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueFamilyIndex = queueIndex;
        createInfo.queueCount = 1;
        createInfo.pQueuePriorities = &queuePriority;

        queueCreateInfo.push_back(createInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfo.size());
    createInfo.pQueueCreateInfos = queueCreateInfo.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = sDeviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));
}

void VulkanContext::setupQueues()
{
    for (uint32_t index : m_queueFamilyIndices.getUniqueIndices())
    {
        Queue queue;

        queue.index = index;
        vkGetDeviceQueue(m_device, index, 0, &queue.object);

        VkCommandPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = index;

        VK_CALL(vkCreateCommandPool(m_device, &createInfo, nullptr, &queue.commandPool));

        m_queues[index] = std::move(queue);
    }
}

void VulkanContext::createDefaultResources()
{
    // Default PBR shaders
    resource::ResourceManager::Create<resource::Shader>("PBR_VERT", "shaders/pbr_vert.spv");
    resource::ResourceManager::Create<resource::Shader>("PBR_FRAG", "shaders/pbr_frag.spv");
}

void VulkanContext::createSwapChain()
{
    m_swapChain = std::make_unique<SwapChain>();
}

void VulkanContext::createPipeline()
{
    m_pipeline = std::make_unique<Pipeline>();
    m_pipeline->setShader(ShaderStage::kVertex, resource::ResourceManager::GetAs<resource::Shader>("PBR_VERT"));
    m_pipeline->setShader(ShaderStage::kFragment, resource::ResourceManager::GetAs<resource::Shader>("PBR_FRAG"));
    m_pipeline->create();

    // The framebuffers can now be created
    m_swapChain->createFramebuffers();
}

} // namespace core
} // namespace jdl
