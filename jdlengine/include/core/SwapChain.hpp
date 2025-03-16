#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

struct SwapChainInfo
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;

    /**
     * @brief Retrieves and returns the swap chain info for the input window surface and physical device.
     */
    static SwapChainInfo Get(VkPhysicalDevice device, VkSurfaceKHR surface);
};

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
     * @brief Returns the selected surface format.
     */
    VkSurfaceFormatKHR getSurfaceFormat() const { return m_surfaceFormat; }

    /**
     * @brief Returns the selected presentation mode.
     */
    VkPresentModeKHR getPresentationMode() const { return m_presentationMode; }

    /**
     * @brief Returns the swap chain extent.
     */
    VkExtent2D getExtent() const { return m_extent; }

    /**
     * @brief Returns the swap chain object handle.
     */
    VkSwapchainKHR getObject() const { return m_swapChain; }

    /**
     * @brief Returns the number of images in this swap chain.
     */
    size_t getNbImages() const { return m_images.size(); }

    /**
     * @brief  Returns a swap chain image.
     * @param  index Image index. Must be less than the number of images.
     * @return Swap chain image, or a null object if the index is invalid.
     */
    VkImage getImage(size_t index) const {
        return index < getNbImages() ? m_images[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief  Returns a swap chain image view.
     * @param  index Image view index. Must be less than the number of images.
     * @return Swap chain image view, or a null object if the index is invalid.
     */
    VkImageView getImageView(size_t index) const {
        return index < getNbImages() ? m_views[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief Creates the swap chain framebuffers.
     */
    void createFramebuffers();

    /**
     * @brief Returns the number of frameuffers.
     * If the framebuffers have been created, this should be equal to getNbImages().
     */
    size_t getNbFramebuffers() const { return m_framebuffers.size(); }

    /**
     * @brief  Returns a swap chain framebuffer.
     * @param  index Framebuffer index. Must be less than the number of framebuffers.
     * @return Swap chain framebuffer, or a null object if the index is invalid.
     */
    VkFramebuffer getFramebuffer(size_t index) const {
        return index < getNbImages() ? m_framebuffers[index] : VK_NULL_HANDLE;
    }

private:
    // Selected surface format
    VkSurfaceFormatKHR m_surfaceFormat;
    // Selected presentation mode
    VkPresentModeKHR m_presentationMode;
    // Extent
    VkExtent2D m_extent;
    // Swapchain handle
    VkSwapchainKHR m_swapChain;

    // Images & Views
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_views;
    std::vector<VkFramebuffer> m_framebuffers;

    // Creates the swap chain
    void createSwapChain(VkDevice device);
    // Retrieves the images
    void retrieveImages(VkDevice device);
    // Creates the image views
    void createViews(VkDevice device);
};

} // namespace core
} // namespace jdl
