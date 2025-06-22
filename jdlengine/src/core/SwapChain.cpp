#include "core/SwapChain.hpp"
#include "core/VulkanContext.hpp"
#include "core/Window.hpp"

#include "utils/Logger.hpp"
#include "utils/VulkanUtils.hpp"


namespace core
{

SwapChain::SwapChain()
{
    configure();
    create();
    createImageViews();
}

SwapChain::~SwapChain()
{
    VkDevice device = VulkanContext::GetDevice();
    
    for (VkImageView view : m_views) {
        vkDestroyImageView(device, view, nullptr);
    }
    vkDestroySwapchainKHR(device, m_swapChain, nullptr);
}

void SwapChain::configure()
{
    VkPhysicalDevice phyDevice = VulkanContext::GetPhysicalDevice();
    VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

    VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phyDevice, surface, &m_capabilities));

    // Surface format
    uint32_t nbFormats;
    VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &nbFormats, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(nbFormats);
    VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &nbFormats, formats.data()));

    m_surfaceFormat = formats[0];
    for (const auto& format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surfaceFormat = format;
            break;
        }
    }

    // Present Modes
    uint32_t nbPresentModes;
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &nbPresentModes, nullptr));

    std::vector<VkPresentModeKHR> presentModes(nbPresentModes);
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &nbPresentModes, presentModes.data()));

    for (VkPresentModeKHR mode : presentModes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_presentMode = mode;
            break;
        }
    }

    // Extent
    if (m_capabilities.currentExtent.width != UINT32_MAX) {
        m_extent = m_capabilities.currentExtent;
    }
    else
    {
        Size framebufferSize = Window::Get().getFramebufferSize();

        m_extent.width = std::clamp(
            static_cast<uint32_t>(framebufferSize.width),
            m_capabilities.minImageExtent.width,
            m_capabilities.maxImageExtent.width
        );
        m_extent.height = std::clamp(
            static_cast<uint32_t>(framebufferSize.height),
            m_capabilities.minImageExtent.height,
            m_capabilities.maxImageExtent.height
        );
    }
}

void SwapChain::create()
{
    uint32_t nbImages = m_capabilities.minImageCount + 1;
    if (m_capabilities.maxImageCount > 0) {
        nbImages = std::min(nbImages, m_capabilities.maxImageCount);
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VulkanContext::GetWindowSurface();
    createInfo.minImageCount = nbImages;
    createInfo.imageFormat = m_surfaceFormat.format;
    createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
    createInfo.imageExtent = m_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = m_capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = m_presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    QueueFamilyIndices queueFamilyIndices = VulkanContext::GetQueueFamilyIndices();
    uint32_t queueIndices[] = {*queueFamilyIndices.graphics, *queueFamilyIndices.present};

    if (queueIndices[0] != queueIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain));

    // Retrieve the swap chain images
    m_images.resize(nbImages);
    VK_CALL(vkGetSwapchainImagesKHR(device, m_swapChain, &nbImages, m_images.data()));
}

void SwapChain::createImageViews()
{
    m_views.reserve(m_images.size());
    for (VkImage image : m_images) {
        m_views.push_back(utils::VulkanUtils::CreateImageView(
            image, m_surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT
        ));
    }
}

} // namespace core
