#pragma once

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanSwapChain : private NonCopyable<VulkanSwapChain>
{
public:
	VulkanSwapChain();
	~VulkanSwapChain();

	/**
	 * @brief Returns the selected surface format.
	 */
	VkFormat getFormat() const { return m_format.format; }

	/**
	 * @brief Returns the images extent.
	 */
	VkExtent2D getExtent() const { return m_extent; }

	/**
	 * @brief Returns the Vulkan swap chain handle.
	 */
	VkSwapchainKHR getHandle() const { return m_swapChain; }

	/**
	 * @brief Returns the swap chain number of images.
	 */
	uint32_t getNbImages() const {
		return static_cast<uint32_t>(m_images.size());
	}

	/**
	 * @brief Returns a swap chain image.
	 * @param index Image index. Must be < getNbImages().
	 * @return Requested image, or VK_NULL_HANDLE if the index is out of range.
	 */
	VkImage getImage(size_t index) const {
		return index < m_images.size() ? m_images[index] : VK_NULL_HANDLE;
	}

	/**
	 * @brief Returns a swap chain image view.
	 * @param index Image view index. Must be < getNbImages().
	 * @return Requested image view, or VK_NULL_HANDLE if the index is out of range.
	 */
	VkImageView getImageView(size_t index) const {
		return index < m_images.size() ? m_imageViews[index] : VK_NULL_HANDLE;
	}

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkSwapchainKHR, m_swapChain);

	VkSurfaceFormatKHR m_format {};
	VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
	VkExtent2D m_extent {};

	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_imageViews;

	void createSwapChain();
	void createImageViews();
};

} // namespace vk
} // namespace jdl
