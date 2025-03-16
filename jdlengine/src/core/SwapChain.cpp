#include "core/SwapChain.hpp"
#include "core/Application.hpp"
#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"
#include "utils/VulkanUtils.hpp"


namespace jdl
{
namespace core
{

/* --- SwapChainInfo STRUCT --- */

SwapChainInfo SwapChainInfo::Get(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainInfo info = {};

    // Capabilities
    VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &info.capabilities));

    // Surface formats
    uint32_t nbFormats;
    VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &nbFormats, nullptr));
    if (nbFormats > 0)
    {
        info.formats.resize(nbFormats);
        VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &nbFormats, info.formats.data()));
    }

    // Presentation modes
    uint32_t nbModes;
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &nbModes, nullptr));
    if (nbModes > 0)
    {
        info.presentModes.resize(nbModes);
        VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &nbModes, info.presentModes.data()));
    }

    return info;
}


/* --- SwapChain CLASS --- */

SwapChain::SwapChain()
    : m_surfaceFormat()
    , m_presentationMode(VK_PRESENT_MODE_FIFO_KHR)
    , m_extent()
    , m_swapChain(VK_NULL_HANDLE)
{
    auto device = VulkanContext::GetDevice();

    createSwapChain(device);
    retrieveImages(device);
    createViews(device);
}

SwapChain::~SwapChain()
{
    auto device = VulkanContext::GetDevice();
    
    for (VkFramebuffer framebuffer : m_framebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    for (VkImageView view : m_views)
    {
        vkDestroyImageView(device, view, nullptr);
    }
    vkDestroySwapchainKHR(device, m_swapChain, nullptr);
}

void SwapChain::createFramebuffers()
{
    VkDevice device = VulkanContext::GetDevice();
    VkRenderPass renderPass = VulkanContext::GetPipeline().getRenderPass();

    m_framebuffers.resize(getNbImages());
    for (auto i = 0; i < getNbImages(); ++i)
    {
        std::vector<VkImageView> views {m_views[i]};

        VkFramebufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = renderPass;
        createInfo.attachmentCount = static_cast<uint32_t>(views.size());
        createInfo.pAttachments = views.data();
        createInfo.width = m_extent.width;
        createInfo.height = m_extent.height;
        createInfo.layers = 1;

        VK_CALL(vkCreateFramebuffer(device, &createInfo, nullptr, &m_framebuffers[i]));
    }
}

void SwapChain::createSwapChain(VkDevice device)
{
    auto windowSurface = VulkanContext::GetWindowSurface();
    auto swapChainInfo = SwapChainInfo::Get(VulkanContext::GetPhysicalDevice(), windowSurface);

    // Surface format
    m_surfaceFormat = swapChainInfo.formats[0];
    for (auto i = 1; i < swapChainInfo.formats.size(); ++i)
    {
        VkSurfaceFormatKHR format = swapChainInfo.formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surfaceFormat = format;
            break;
        }
    }

    // Presentation mode
    for (VkPresentModeKHR mode : swapChainInfo.presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_presentationMode = mode;
            break;
        }
    }

    // Extent
    if (swapChainInfo.capabilities.currentExtent.width != UINT32_MAX)
    {
        m_extent = swapChainInfo.capabilities.currentExtent;
    }
    else
    {
        // Match if possible the window pixel resolution
        Size windowSize = Application::GetWindow().getFramebufferSize();

        m_extent.width = std::clamp(
            static_cast<uint32_t>(windowSize.width),
            swapChainInfo.capabilities.minImageExtent.width,
            swapChainInfo.capabilities.maxImageExtent.width
        );
        m_extent.height = std::clamp(
            static_cast<uint32_t>(windowSize.height),
            swapChainInfo.capabilities.minImageExtent.height,
            swapChainInfo.capabilities.maxImageExtent.height
        );
    }

    // Image count
    uint32_t nbImages = swapChainInfo.capabilities.minImageCount + 1;
    if (swapChainInfo.capabilities.maxImageCount > 0 && nbImages > swapChainInfo.capabilities.maxImageCount)
    {
        nbImages = swapChainInfo.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = windowSurface;
    createInfo.minImageCount = nbImages;
    createInfo.imageFormat = m_surfaceFormat.format;
    createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
    createInfo.presentMode = m_presentationMode;
    createInfo.imageExtent = m_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = swapChainInfo.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    const auto& queueFamilyIndices = VulkanContext::GetQueueFamilyIndices();
    uint32_t queueIndices[] = {*queueFamilyIndices.graphics, *queueFamilyIndices.present};

    if (queueIndices[0] != queueIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VK_CALL(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain));
}

void SwapChain::retrieveImages(VkDevice device)
{
    uint32_t nbImages;

    vkGetSwapchainImagesKHR(device, m_swapChain, &nbImages, nullptr);
    m_images.resize(nbImages);
    vkGetSwapchainImagesKHR(device, m_swapChain, &nbImages, m_images.data());
}

void SwapChain::createViews(VkDevice device)
{
    VkFormat format = m_surfaceFormat.format;
    VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    m_views.reserve(m_images.size());
    for (VkImage image : m_images)
    {
        m_views.push_back(utils::VulkanUtils::CreateImageView(device, image, format, aspectMask));
    }
}

} // namespace core
} // namespace jdl
