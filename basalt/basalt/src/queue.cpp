#include "queue.h"

#include <stdexcept>

#include "device.h"
#include "swapchain.h"

namespace basalt {

    Queue::Queue(Device& device, const uint32_t graphicsQueueFamilyIndex, const uint32_t presentQueueFamilyIndex)
        : device(device)
    {
        vkGetDeviceQueue(device.getDevice(), graphicsQueueFamilyIndex, 0, &graphicsQueue);
        vkGetDeviceQueue(device.getDevice(), presentQueueFamilyIndex, 0, &presentQueue);
        if (graphicsQueue == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to get graphics queue!");
        }
        if (presentQueue == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to get present queue!");
        }
    }

    Queue::~Queue()
    {
        // No resources to clean up for queues
    }

    VkResult Queue::submitCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers,
        const std::vector<VkSemaphore>& waitSemaphores,
        const std::vector<VkPipelineStageFlags>& waitStages,
        const std::vector<VkSemaphore>& signalSemaphores,
        const VkFence fence) const
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        // Wait semaphores
        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();

        // Command buffers
        submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        submitInfo.pCommandBuffers = commandBuffers.data();

        // Signal semaphores
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        return vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);
    }

    VkResult Queue::presentImage(const SwapChain& swapChain, const uint32_t imageIndex,
                                 const std::vector<VkSemaphore>& waitSemaphores) const
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        // Wait semaphores
        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        presentInfo.pWaitSemaphores = waitSemaphores.data();

        // Swap chains and image indices
        const VkSwapchainKHR swapChains[] = { swapChain.getSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        // Present the image
        return vkQueuePresentKHR(presentQueue, &presentInfo);
    }

} // namespace basalt
