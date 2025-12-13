#include "vulkan/VulkanSwapChain.hpp"
#include "vulkan/VulkanContext.hpp"
#include "vulkan/VulkanDevice.hpp"

#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanSwapChain::VulkanSwapChain()
{
	m_device = VulkanContext::GetDevice().get();

	createSwapChain();
	createImageViews();
}

VulkanSwapChain::~VulkanSwapChain()
{
	for (VkFramebuffer framebuffer : m_framebuffers) {
		vkDestroyFramebuffer(m_device, framebuffer, nullptr);
	}
	for (VkImageView view : m_views) {
		vkDestroyImageView(m_device, view, nullptr);
	}
	vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void VulkanSwapChain::createFramebuffers(VkRenderPass renderPass)
{
	if (!m_framebuffers.empty()) {
		return;
	}
	m_framebuffers.resize(m_images.size());

	for (auto i = 0; i < m_views.size(); ++i)
	{
		VkImageView attachments[] = { m_views[i] };

		VkFramebufferCreateInfo framebufferInfo {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = m_extent.width;
		framebufferInfo.height = m_extent.height;
		framebufferInfo.layers = 1;

		VK_CALL(vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]));
	}
}

void VulkanSwapChain::createSwapChain()
{
	VkPhysicalDevice phyDevice = VulkanContext::GetDevice().getPhysicalDevice();
	VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

	VkSurfaceCapabilitiesKHR capabilities;
	VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phyDevice, surface, &capabilities));

	// Surface format
	getSurfaceFormat(phyDevice, surface);
	// Present mode
	getPresentMode(phyDevice, surface);

	// Extent
	if (capabilities.currentExtent.width != UINT32_MAX) {
		m_extent = capabilities.currentExtent;
	}
	else
	{
		core::Size windowSize = core::Window::Get().getFramebufferSize();
		
		m_extent.width = std::clamp(
			static_cast<uint32_t>(windowSize.width),
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		m_extent.height = std::clamp(
			static_cast<uint32_t>(windowSize.height),
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);
	}

	uint32_t nbImages = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0) {
		nbImages = std::min(nbImages, capabilities.maxImageCount);
	}

	VkSwapchainCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.clipped = VK_TRUE;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.imageArrayLayers = 1;
	createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageFormat = m_surfaceFormat.format;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.minImageCount = nbImages;
	createInfo.presentMode = m_presentMode;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.surface = surface;

	const QueueFamilyIndices& queueFamilyIndices = VulkanContext::GetDevice().getQueueFamilyIndices();
	uint32_t indices[] = { queueFamilyIndices.graphics, queueFamilyIndices.present };

	if (queueFamilyIndices.graphics == queueFamilyIndices.present) {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = indices;
	}

	VK_CALL(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain));

	// Retrieve the swap chain images
	m_images.resize(nbImages);
	VK_CALL(vkGetSwapchainImagesKHR(m_device, m_swapChain, &nbImages, m_images.data()));
}

void VulkanSwapChain::createImageViews()
{
	size_t nbImages = m_images.size();
	m_views.resize(nbImages);

	for (auto i = 0; i < nbImages; ++i)
	{
		VkImageViewCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.format = m_surfaceFormat.format;
		createInfo.image = m_images[i];
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.layerCount = 1;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

		VK_CALL(vkCreateImageView(m_device, &createInfo, nullptr, &m_views[i]));
	}
}

void VulkanSwapChain::getSurfaceFormat(VkPhysicalDevice phyDevice, VkSurfaceKHR surface)
{
	uint32_t nbFormats = 0;
	VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &nbFormats, nullptr));

	std::vector<VkSurfaceFormatKHR> formats(nbFormats);
	VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevice, surface, &nbFormats, formats.data()));

	m_surfaceFormat = formats[0];
	for (uint32_t i = 1; i < nbFormats; ++i)
	{
		VkSurfaceFormatKHR format = formats[i];
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			m_surfaceFormat = format;
			return;
		}
	}
}

void VulkanSwapChain::getPresentMode(VkPhysicalDevice phyDevice, VkSurfaceKHR surface)
{
	uint32_t nbModes = 0;
	VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &nbModes, nullptr));

	std::vector<VkPresentModeKHR> modes(nbModes);
	VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(phyDevice, surface, &nbModes, modes.data()));

	for (VkPresentModeKHR mode : modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			m_presentMode = mode;
			return;
		}
	}
}

} // namespace vk
} // namespace jdl
