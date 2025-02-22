#include "core/VulkanContext.hpp"
#include "core/Application.hpp"
#include "core/Window.hpp"

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


/* --- VulkanContext CLASS --- */

VulkanContext VulkanContext::IContext;

VulkanContext::VulkanContext()
    : m_instance(VK_NULL_HANDLE)
    , m_debugMessenger(VK_NULL_HANDLE)
{}

void VulkanContext::init()
{
    if (m_instance == VK_NULL_HANDLE)
    {
        createInstance();
        createDebugMessenger();
    }
}

void VulkanContext::destroy()
{
    if (m_instance != VK_NULL_HANDLE)
    {
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

} // namespace core
} // namespace jdl
