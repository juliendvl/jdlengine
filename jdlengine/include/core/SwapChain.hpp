#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace core
{

class JDL_API SwapChain : private NonCopyable<SwapChain>
{
public:
    /**
     * @brief Creates the swap chain.
     */
    SwapChain();

    /**
     * @brief Destroys the swap chain.
     */
    ~SwapChain();

    /**
     * @brief Returns the swap chain surface format.
     */
    VkSurfaceFormatKHR getSurfaceFormat() const {
        return m_surfaceFormat;
    }

    /**
     * @brief Returns the swap chain extent.
     */
    VkExtent2D getExtent() const {
        return m_extent;
    }

    /**
     * @brief Returns the swap chain handle.
     */
    VkSwapchainKHR getHandle() const {
        return m_swapChain;
    }

    /**
     * @brief Returns the number of images in the swap chain.
     */
    size_t getNbImages() const {
        return m_images.size();
    }

    /**
     * @brief  Returns a swap chain image.
     * @param  index Image index. Must be < getNbImages()
     * @return Image, or VK_NULL_HANDLE if the index is invalid.
     */
    VkImage getImage(size_t index) const {
        return index < getNbImages() ? m_images[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief  Returns a swap chain image view.
     * @param  index Image view index. Must be < getNbImages()
     * @return Image view, or VK_NULL_HANDLE if the index is invalid.
     */
    VkImageView getImageView(size_t index) const {
        return index < getNbImages() ? m_views[index] : VK_NULL_HANDLE;
    }

private:
    // Swap chain capabilities
    VkSurfaceCapabilitiesKHR m_capabilities;
    // Images format
    VkSurfaceFormatKHR m_surfaceFormat;
    // Presentation mode
    VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    // Images extent
    VkExtent2D m_extent;
    // SwapChain handle
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

    // Images
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_views;
    
    void configure();
    void create();
    void createImageViews();
};

} // namespace core
