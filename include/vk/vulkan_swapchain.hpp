#pragma once

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanSwapchain : private NonCopyable<VulkanSwapchain>
{
public:
	VulkanSwapchain();
	~VulkanSwapchain();

	/**
	 * @brief Returns the selected surface format.
	 */
	VkSurfaceFormatKHR get_surface_format() const { return m_surfaceFormat; }

	/**
	 * @brief Returns the selected extent.
	 */
	VkExtent2D get_extent() const { return m_extent; }

	/**
	 * @brief Returns the swapchain Vulkan handle.
	 */
	VkSwapchainKHR get_handle() const { return m_swapchain; }

	/**
	 * @brief Returns the number of images in the swapchain.
	 */
	size_t get_nb_images() const { return m_images.size(); }

	/**
	 * @brief Returns a swapchain image
	 * @param index Swapchain image index
	 * @return The queried image, or VK_NULL_HANDLE if the index is invalid
	 */
	VkImage get_image(size_t index) const {
		return index < m_images.size() ? m_images[index] : VK_NULL_HANDLE;
	}

	/**
	 * @brief Returns a swapchain image view
	 * @param index Swapchain image view index
	 * @return The queried image view, or VK_NULL_HANDLE if the index is invalid
	 */
	VkImageView get_image_view(size_t index) const {
		return index < m_images.size() ? m_imageViews[index] : VK_NULL_HANDLE;
	}

private:
	VkDevice m_device = VK_NULL_HANDLE;
	VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

	VkSurfaceFormatKHR m_surfaceFormat {};
	VkExtent2D m_extent {};

	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_imageViews;

	void create_swapchain();
	void create_image_views();
};

} // namespace vk
} // namespace jdl
