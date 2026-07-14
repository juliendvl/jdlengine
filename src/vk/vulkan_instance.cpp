#include "vk/vulkan_instance.hpp"

#include "core/application.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

// --- VALIDATION LAYERS ---

#ifndef NDEBUG
static constexpr bool s_UseValidationLayers = true;
#else
static constexpr bool s_UseValidationLayers = false;
#endif

const std::vector<const char*> s_ValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static void s_CheckValidationLayersSupport()
{
    uint32_t nb_layers;
    VK_CALL(vkEnumerateInstanceLayerProperties(&nb_layers, nullptr));

    std::vector<VkLayerProperties> layers(nb_layers);
    VK_CALL(vkEnumerateInstanceLayerProperties(&nb_layers, VK_DATA(layers)));

    for (const char* required_layer : s_ValidationLayers)
    {
        bool layer_found = false;
        for (const auto& layer : layers)
        {
            if (strcmp(layer.layerName, required_layer) == 0)
            {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            JDL_FATAL("Validation layer {} not supported", required_layer);
        }
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* user_data
) {
    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            JDL_TRACE("{}", data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            JDL_INFO("{}", data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            JDL_WARN("{}", data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            JDL_ERROR("{}", data->pMessage);
            break;
        }
        default: {
            break;
        }
    }

    return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT s_DebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = s_DebugCallback;

    return create_info;
}

static VkResult s_CreateDebugMessenger(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* debug_messenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT"
    );

    if (func != nullptr) {
        return func(instance, create_info, allocator, debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void s_DestroyDebugMessenger(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks* allocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT"
    );
    if (func != nullptr) {
        func(instance, debug_messenger, allocator);
    }
}


// --- VulkanInstance CLASS ---

VulkanInstance::VulkanInstance()
{
    create_instance();
    create_debug_messenger();
}

VulkanInstance::~VulkanInstance()
{
    if (m_debugMessenger != VK_NULL_HANDLE) {
        s_DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
    }
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanInstance::create_instance()
{
    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = core::Application::GetName();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "JDLEngine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    auto extensions = core::Application::GetWindow().GetRequiredInstanceExtensions();
    if (s_UseValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = VK_SIZE(extensions);
    create_info.ppEnabledExtensionNames = VK_DATA(extensions);

    auto debug_messenger_info = s_DebugMessengerCreateInfo();
    if (s_UseValidationLayers)
    {
        create_info.enabledLayerCount = VK_SIZE(s_ValidationLayers);
        create_info.ppEnabledLayerNames = VK_DATA(s_ValidationLayers);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_messenger_info;
    }
    else {
        create_info.enabledLayerCount = 0;
    }

    VK_CALL(vkCreateInstance(&create_info, nullptr, &m_instance));
}

void VulkanInstance::create_debug_messenger()
{
    if (!s_UseValidationLayers) {
        return;
    }

    auto create_info = s_DebugMessengerCreateInfo();
    VK_CALL(s_CreateDebugMessenger(
        m_instance, &create_info, nullptr, &m_debugMessenger
    ));
}

} // namespace vk
} // namespace jdl
