#include "vk/VulkanSwapChain.hpp"
#include "vk/VulkanContext.hpp"

#include "core/Application.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanSwapChain::VulkanSwapChain()
{
	m_device = VulkanContext::GetDevice().getDevice();

	createSwapChain();
	createImageViews();
}

VulkanSwapChain::~VulkanSwapChain()
{
	for (VkImageView imageView : m_imageViews) {
		vkDestroyImageView(m_device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void VulkanSwapChain::createSwapChain()
{
	VkPhysicalDevice phyDevice = VulkanContext::GetDevice().getPhysicalDevice();
	VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

	VkSurfaceCapabilitiesKHR capabilities;
	VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phyDevice, surface, &capabilities));

	// Surface format
	uint32_t nbFormats = 0;
	VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(
		phyDevice, surface,
		&nbFormats, nullptr
	));

	std::vector<VkSurfaceFormatKHR> formats(nbFormats);
	VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(
		phyDevice, surface,
		&nbFormats, formats.data()
	));

	m_format = formats[0];
	for (uint32_t i = 1; i < nbFormats; ++i)
	{
		const auto& format = formats[i];

		if (format.format != VK_FORMAT_B8G8R8A8_SRGB) {
			continue;
		}
		if (format.colorSpace != VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			continue;
		}

		m_format = formats[i];
		break;
	}

	// Presentation mode
	uint32_t nbModes = 0;
	VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(
		phyDevice, surface,
		&nbModes, nullptr
	));

	std::vector<VkPresentModeKHR> modes(nbModes);
	VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(
		phyDevice, surface,
		&nbModes, modes.data()
	));

	for (VkPresentModeKHR mode : modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			m_presentMode = mode;
			break;
		}
	}

	// Extent
	if (capabilities.currentExtent.width != UINT32_MAX) {
		m_extent = capabilities.currentExtent;
	}
	else
	{
		core::Size framebufferSize = core::Application::GetWindow().getFramebufferSize();

		m_extent.width = std::clamp(
			static_cast<uint32_t>(framebufferSize.width),
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		m_extent.height = std::clamp(
			static_cast<uint32_t>(framebufferSize.height),
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);
	}

	uint32_t nbImages = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && nbImages > capabilities.maxImageCount) {
		nbImages = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = nbImages;
	createInfo.imageFormat = m_format.format;
	createInfo.imageColorSpace = m_format.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = m_presentMode;
	createInfo.clipped = VK_TRUE;

	const auto& queueFamilies = VulkanContext::GetDevice().getQueueFamilyIndices();
	uint32_t indices[] = {queueFamilies.graphics, queueFamilies.present};

	if (indices[0] != indices[1])
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = indices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	VK_CALL(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain));

	m_images.resize(nbImages);
	VK_CALL(vkGetSwapchainImagesKHR(m_device, m_swapChain, &nbImages, m_images.data()));
}

void VulkanSwapChain::createImageViews()
{
	VkImageViewCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.format = m_format.format;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	m_imageViews.resize(m_images.size());
	for (auto i = 0; i < m_images.size(); ++i)
	{
		createInfo.image = m_images[i];
		VK_CALL(vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]));
	}
}

} // namespace vk
} // namespace jdl
