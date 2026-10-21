#include "vk/vk_swapchain.hpp"

#include <algorithm>
#include <array>

#include "core/application.hpp"
#include "core/window.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

namespace
{

VkSurfaceFormatKHR ChooseSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t nb_formats = 0;
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &nb_formats, nullptr));

	std::vector<VkSurfaceFormatKHR> formats(nb_formats);
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
		device, surface, &nb_formats, VK_DATA(formats)
	));

	// sRGB format in priority, otherwise the first available one.
	for (const auto& format : formats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}
	return formats[0];
}

VkPresentModeKHR ChoosePresentMode(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t nb_modes = 0;
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
		device, surface, &nb_modes, nullptr
	));

	std::vector<VkPresentModeKHR> modes(nb_modes);
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
		device, surface, &nb_modes, VK_DATA(modes)
	));

	// Mailbox in priority, otherwise FIFO (always supported).
	for (VkPresentModeKHR mode : modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return mode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// The surface size is imposed by the window manager.
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}

	// Otherwise, match the window framebuffer size within the supported bounds.
	core::Size size = core::Application::GetWindow().framebuffer_size();

	return VkExtent2D {
		.width = std::clamp(
			static_cast<uint32_t>(size.width),
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		),
		.height = std::clamp(
			static_cast<uint32_t>(size.height),
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		)
	};
}

} // namespace

Swapchain::Swapchain(const Instance& instance, const Device& device)
	: m_device(device.device())
{
	create_swapchain(instance.surface(), device);
	create_image_views();

	JDL_INFO("Created swapchain ({} images, {}x{})",
		m_images.size(), m_extent.width, m_extent.height);
}

Swapchain::~Swapchain()
{
	for (VkImageView image_view : m_imageViews) {
		vkDestroyImageView(m_device, image_view, nullptr);
	}
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

std::optional<uint32_t> Swapchain::acquire_next_image(VkSemaphore semaphore) const
{
	uint32_t image_index = 0;

	VkResult result = vkAcquireNextImageKHR(
		m_device, m_swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &image_index
	);

	// A suboptimal swapchain can still be used for presentation.
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		return std::nullopt;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		JDL_FATAL("Failed to acquire a swapchain image (VkResult {})", static_cast<int>(result));
	}
	return image_index;
}

void Swapchain::create_swapchain(VkSurfaceKHR surface, const Device& device)
{
	VkPhysicalDevice physical_device = device.physical_device();

	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		physical_device, surface, &capabilities
	));

	VkSurfaceFormatKHR surface_format = ChooseSurfaceFormat(physical_device, surface);
	VkPresentModeKHR present_mode = ChoosePresentMode(physical_device, surface);
	m_imageFormat = surface_format.format;
	m_extent = ChooseExtent(capabilities);

	// One more image than the minimum, so as not to wait on the driver.
	// A maximum of 0 means there is no limit.
	uint32_t nb_images = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0) {
		nb_images = std::min(nb_images, capabilities.maxImageCount);
	}

	VkSwapchainCreateInfoKHR create_info {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = nb_images,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = m_extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = capabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = present_mode,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
	};

	// Images are shared when graphics and presentation use distinct queue families.
	const QueueFamilyIndices& queue_indices = device.queue_family_indices();
	std::array<uint32_t, 2> family_indices = {
		queue_indices.graphics, queue_indices.present
	};

	if (queue_indices.graphics != queue_indices.present)
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = VK_SIZE(family_indices);
		create_info.pQueueFamilyIndices = VK_DATA(family_indices);
	}

	VK_CHECK(vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain));

	// The implementation may create more images than requested.
	VK_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &nb_images, nullptr));
	m_images.resize(nb_images);
	VK_CHECK(vkGetSwapchainImagesKHR(
		m_device, m_swapchain, &nb_images, VK_DATA(m_images)
	));
}

void Swapchain::create_image_views()
{
	m_imageViews.resize(m_images.size(), VK_NULL_HANDLE);

	for (size_t i = 0; i < m_images.size(); ++i)
	{
		VkImageViewCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = m_imageFormat,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};
		VK_CHECK(vkCreateImageView(m_device, &create_info, nullptr, &m_imageViews[i]));
	}
}

} // namespace vk
} // namespace jdl
