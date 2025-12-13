#include "vulkan/VulkanContext.hpp"

#include "core/Application.hpp"
#include "core/Window.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Shader.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

// --- Validation Layers ---
static const std::vector<const char*> sValidationLayers{
    "VK_LAYER_KHRONOS_validation"
};

#ifndef NDEBUG
static const bool sUseValidationLayers = true;
#else
static const bool sUseValidationLayers = false;
#endif // !NDEBUG

static void s_CheckValidationLayers()
{
    uint32_t nbLayers = 0;
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, nullptr));

    std::vector<VkLayerProperties> layers(nbLayers);
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, layers.data()));

    for (const char* requiredLayer : sValidationLayers)
    {
        bool layerFound = false;
        for (const auto& layer : layers)
        {
            if (strcmp(requiredLayer, layer.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            JDL_FATAL("Validation layer {} is not supported", requiredLayer);
        }
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* pUserData
)
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

static VkResult s_CreateDebugMessenger(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
)
{
    auto fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void s_DestroyDebugMessenger(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
)
{
    auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        fn(instance, debugMessenger, pAllocator);
    }
}


// --- VulkanContext ---
VulkanContext VulkanContext::CONTEXT;

void VulkanContext::doInit()
{
    if (m_instance != VK_NULL_HANDLE) {
        return;
    }

    createInstance();
    createDebugMessenger();
    createWindowSurface();
    createDevice();
    createSwapChain();
    createDefaultResources();
    createPipeline();
}

void VulkanContext::doDestroy()
{
    if (m_instance == VK_NULL_HANDLE) {
        return;
    }

    m_pipeline.reset();
    m_swapChain.reset();
    m_device.reset();

    vkDestroySurfaceKHR(m_instance, m_windowSurface, nullptr);

    if (m_debugMessenger != VK_NULL_HANDLE) {
        s_DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
    }

    vkDestroyInstance(m_instance, nullptr);
    m_instance = VK_NULL_HANDLE;
}

void VulkanContext::createInstance()
{
    if (sUseValidationLayers) {
        s_CheckValidationLayers();
    }
    auto debugMessengerInfo = s_DebugMessengerCreateInfo();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = core::Application::GetName().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> extensions = core::Window::GetInstanceExtensions();
    if (sUseValidationLayers) {
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
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

void VulkanContext::createDebugMessenger()
{
    if (!sUseValidationLayers) {
        return;
    }

    auto createInfo = s_DebugMessengerCreateInfo();
    VK_CALL(s_CreateDebugMessenger(m_instance, &createInfo, nullptr, &m_debugMessenger));
}

void VulkanContext::createWindowSurface()
{
    GLFWwindow* window = core::Window::Get().getHandle();
    VK_CALL(glfwCreateWindowSurface(m_instance, window, nullptr, &m_windowSurface));
}

void VulkanContext::createDevice()
{
    m_device = std::make_unique<VulkanDevice>();
}

void VulkanContext::createSwapChain()
{
    m_swapChain = std::make_unique<VulkanSwapChain>();
}

void VulkanContext::createDefaultResources()
{
    resource::ResourceManager::Create<resource::Shader>("default_vert", "shaders/default_vert.spv");
    resource::ResourceManager::Create<resource::Shader>("default_frag", "shaders/default_frag.spv");
}

void VulkanContext::createPipeline()
{
    m_pipeline = std::make_unique<VulkanPipeline>();
    m_pipeline->addShader(ShaderStage::eVertex, resource::ResourceManager::GetAs<resource::Shader>("default_vert"));
    m_pipeline->addShader(ShaderStage::eFragment, resource::ResourceManager::GetAs<resource::Shader>("default_frag"));
    m_pipeline->create();

    // Create the swap chain framebuffers
    m_swapChain->createFramebuffers(m_pipeline->getRenderPass());
}

} // namespace vk
} // namespace jdl
