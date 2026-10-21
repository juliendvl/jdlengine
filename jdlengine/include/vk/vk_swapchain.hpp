#pragma once

#include <optional>
#include <vector>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_device.hpp"
#include "vk_instance.hpp"


namespace jdl
{
namespace vk
{

class Swapchain : private NonCopyable<Swapchain>
{
public:
	/**
	 * @brief Creates the swapchain for the window surface, along with a view for
	 * each of its images.
	 * @param instance The Vulkan instance owning the window surface.
	 * @param device The device the swapchain is created for.
	 */
	Swapchain(const Instance& instance, const Device& device);

	/**
	 * @brief Destroys the image views and the swapchain.
	 */
	~Swapchain();

	/**
	 * @brief Returns the swapchain handle.
	 */
	VkSwapchainKHR handle() const { return m_swapchain; }

	/**
	 * @brief Returns the format of the swapchain images.
	 */
	VkFormat image_format() const { return m_imageFormat; }

	/**
	 * @brief Returns the extent of the swapchain images, in pixels.
	 */
	VkExtent2D extent() const { return m_extent; }

	/**
	 * @brief Returns the number of swapchain images.
	 */
	uint32_t image_count() const { return VK_SIZE(m_images); }

	/**
	 * @brief Returns a swapchain image.
	 * @param index The image index, lower than image_count().
	 */
	VkImage image(uint32_t index) const { return m_images[index]; }

	/**
	 * @brief Returns the view of a swapchain image.
	 * @param index The image index, lower than image_count().
	 */
	VkImageView image_view(uint32_t index) const { return m_imageViews[index]; }

	/**
	 * @brief Acquires the next available swapchain image, waiting for it if necessary.
	 * @param semaphore The semaphore signaled once the image can actually be used.
	 * @return The index of the acquired image, or std::nullopt if the swapchain is out of
	 * date and must be recreated.
	 */
	std::optional<uint32_t> acquire_next_image(VkSemaphore semaphore) const;

private:
	// Required to destroy the owned objects
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkSwapchainKHR, m_swapchain);

	VkFormat m_imageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D m_extent {};

	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_imageViews;

	void create_swapchain(VkSurfaceKHR surface, const Device& device);
	void create_image_views();
};

} // namespace vk
} // namespace jdl
