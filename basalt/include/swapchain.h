#pragma once

#include <vector>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;    // Forward declaration
    class Surface;   // Forward declaration
    class RenderPass; // Forward declaration
    class SyncObjects; // Forward declaration

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    class SwapChain {
    public:
        SwapChain(Device& device, Surface& surface, GLFWwindow* window);
        ~SwapChain();

        // Delete copy/move
        SwapChain(SwapChain&) = delete;
        SwapChain(SwapChain&&) = delete;
        SwapChain& operator= (const SwapChain&) = delete;
        SwapChain&& operator= (const SwapChain&&) = delete;

        // Accessors
        VkSwapchainKHR getSwapChain() const { return swapChain; }
        VkFormat getImageFormat() const { return imageFormat; }
        VkExtent2D getExtent() const { return extent; }
        const std::vector<VkImageView>& getImageViews() const { return imageViews; }
        const std::vector<VkFramebuffer>& getFramebuffers() const { return framebuffers; }
        size_t getImageCount() const { return swapChainImages.size(); }
        const std::vector<VkImage>& getImages() const { return swapChainImages; }

        // Methods
        void createFramebuffers(const RenderPass& renderPass);
        void recreateSwapChain(Device& device, Surface& surface, GLFWwindow* window);

        // Synchronization and presentation methods
        VkResult acquireNextImage(const SyncObjects& syncObjects, uint32_t currentFrame, uint32_t& imageIndex) const;
        VkResult presentImage(const SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex) const;

        // Cleanup
        void cleanup();

    private:
        Device& device;
        Surface& surface;
        GLFWwindow* window;

        VkSwapchainKHR swapChain;
        VkFormat imageFormat;
        VkExtent2D extent;

        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;

        // Methods
        void createSwapChain();
        void createImageViews();

        // Helper methods
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
    };

} // namespace basalt
