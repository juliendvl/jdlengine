#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class JDL_API VulkanSwapChain : private NonCopyable<VulkanSwapChain>
{
public:
	/**
	 * @brief Creates the swap chain.
	 */
	VulkanSwapChain();

	/**
	 * @brief Destroys the swap chain.
	 */
	~VulkanSwapChain();

	/**
	 * @brief Returns the selected surface format.
	 */
	VkSurfaceFormatKHR getSurfaceFormat() const {
		return m_surfaceFormat;
	}

	/**
	 * @brief Returns the selected extent.
	 */
	VkExtent2D getExtent() const {
		return m_extent;
	}

	/**
	 * @brief Returns the Vulkan swap chain handle.
	 */
	VkSwapchainKHR get() const {
		return m_swapChain;
	}

	/**
	 * @brief Returns the number of images.
	 */
	size_t getNbImages() const {
		return m_images.size();
	}

	/**
	 * @brief Returns a swapchain image.
	 * @param index Image index. Must be < getNbImages().
	 * @return The queried image.
	 */
	VkImage getImage(size_t index) const {
		return m_images[index];
	}

	/**
	 * @brief Returns a swapchain image view.
	 * @param index Image view index. Must be < getNbImages().
	 * @return The queried image view.
	 */
	VkImageView getImageView(size_t index) const {
		return m_views[index];
	}

	/**
	 * @brief Creates the framebuffers for the swap chain images.
	 * @param renderPass The render pass on which the framebuffers must be bound.
	 */
	void createFramebuffers(VkRenderPass renderPass);

private:
	// Source device
	VkDevice m_device = VK_NULL_HANDLE;

	// SwapChain parameters
	VkSurfaceFormatKHR m_surfaceFormat;
	VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
	VkExtent2D m_extent;

	// SwapChain handle
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	// Images and Views
	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_views;

	// Framebuffers
	std::vector<VkFramebuffer> m_framebuffers;

	void createSwapChain();
	void createImageViews();
	void getSurfaceFormat(VkPhysicalDevice phyDevice, VkSurfaceKHR surface);
	void getPresentMode(VkPhysicalDevice phyDevice, VkSurfaceKHR surface);
};

} // namespace vk
} // namespace jdl
