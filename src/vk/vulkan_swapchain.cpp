#include "vk/vulkan_swapchain.hpp"
#include "vk/vulkan_context.hpp"
#include "vk/vulkan_device.hpp"

#include "core/window.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

VulkanSwapchain::VulkanSwapchain()
{
	m_device = VulkanContext::GetDevice().get_device();

	create_swapchain();
	create_image_views();
}

VulkanSwapchain::~VulkanSwapchain()
{
	for (VkImageView image_view : m_imageViews) {
		vkDestroyImageView(m_device, image_view, nullptr);
	}
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

void VulkanSwapchain::create_swapchain()
{
	auto physical_device = VulkanContext::GetDevice().get_physical_device();
	auto surface = VulkanContext::GetWindowSurface();

	VkSurfaceCapabilitiesKHR capabilities;
	VK_CALL(
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
			physical_device, surface, &capabilities
		)
	);
	
	// Surface Format selection
	uint32_t nb_formats = 0;
	VK_CALL(
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			physical_device, surface, &nb_formats, nullptr
		)
	);

	std::vector<VkSurfaceFormatKHR> formats(nb_formats);
	VK_CALL(
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			physical_device, surface, &nb_formats, formats.data()
		)
	);

	m_surfaceFormat = formats[0];
	for (uint32_t i = 1; i < nb_formats; ++i)
	{
		VkSurfaceFormatKHR format = formats[i];
		if (
			format.format == VK_FORMAT_B8G8R8A8_SRGB &&
			format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		)
		{
			m_surfaceFormat = format;
			break;
		}
	}

	// Present mode selection
	uint32_t nb_modes = 0;
	VK_CALL(
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			physical_device, surface, &nb_modes, nullptr
		)
	);

	std::vector<VkPresentModeKHR> modes(nb_modes);
	VK_CALL(
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			physical_device, surface, &nb_modes, modes.data()
		)
	);

	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (VkPresentModeKHR mode : modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			present_mode = mode;
			break;
		}
	}

	// Extent selection
	if (capabilities.currentExtent.width != UINT32_MAX) {
		m_extent = capabilities.currentExtent;
	}
	else
	{
		core::Size framebuffer_size = core::Window::Get().get_framebuffer_size();

		m_extent.width = std::clamp(
			static_cast<uint32_t>(framebuffer_size.width),
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		m_extent.height = std::clamp(
			static_cast<uint32_t>(framebuffer_size.height),
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);
	}

	// Swapchain images count
	uint32_t nb_images = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && nb_images > capabilities.maxImageCount) {
		nb_images = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;
	create_info.minImageCount = nb_images;
	create_info.imageFormat = m_surfaceFormat.format;
	create_info.imageColorSpace = m_surfaceFormat.colorSpace;
	create_info.imageExtent = m_extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	create_info.preTransform = capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	const auto& queue_families = VulkanContext::GetDevice().get_queue_family_indices();
	uint32_t indices[] = {queue_families.graphics, queue_families.present};

	if (indices[0] != indices[1])
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = indices;
	}
	else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	VK_CALL(vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain));

	m_images.resize(nb_images);
	VK_CALL(
		vkGetSwapchainImagesKHR(
			m_device, m_swapchain, &nb_images, m_images.data()
		)
	);
}

void VulkanSwapchain::create_image_views()
{
	VkImageViewCreateInfo create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info.format = m_surfaceFormat.format;
	create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	create_info.subresourceRange.baseMipLevel = 0;
	create_info.subresourceRange.levelCount = 1;
	create_info.subresourceRange.baseArrayLayer = 0;
	create_info.subresourceRange.layerCount = 1;
	
	m_imageViews.resize(m_images.size());

	for (auto i = 0; i < m_images.size(); ++i)
	{
		create_info.image = m_images[i];
		VK_CALL(vkCreateImageView(m_device, &create_info, nullptr, &m_imageViews[i]));
	}
}

} // namespace vk
} // namespace jdl
