#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;       // Forward declaration
    class SwapChain;    // Forward declaration

    class Queue {
    public:
        Queue(Device& device, uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex);
        ~Queue();

        // Delete copy/move
        Queue(Queue&) = delete;
        Queue(Queue&&) = delete;
        Queue& operator= (const Queue&) = delete;
        Queue&& operator= (const Queue&&) = delete;

        // Methods for submitting command buffers
        VkResult submitCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers,
            const std::vector<VkSemaphore>& waitSemaphores,
            const std::vector<VkPipelineStageFlags>& waitStages,
            const std::vector<VkSemaphore>& signalSemaphores,
            VkFence fence = VK_NULL_HANDLE) const;

        // Method for presenting swap chain images
        VkResult presentImage(const SwapChain& swapChain, uint32_t imageIndex,
                              const std::vector<VkSemaphore>& waitSemaphores) const;

        // Accessors
        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkQueue getPresentQueue() const { return presentQueue; }

    private:
        Device& device;

        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
    };

} // namespace basalt
