#include "sync_objects.h"

#include <stdexcept>

#include "device.h"

namespace basalt {

    SyncObjects::SyncObjects(Device& device, const uint32_t maxFramesInFlight)
        : device(device), maxFramesInFlight(maxFramesInFlight)
    {
        createSyncObjects();
    }

    SyncObjects::~SyncObjects()
    {
	    const VkDevice vkDevice = device.getDevice();

        for (size_t i = 0; i < maxFramesInFlight; ++i) {
            if (imageAvailableSemaphores[i] != VK_NULL_HANDLE) {
                vkDestroySemaphore(vkDevice, imageAvailableSemaphores[i], nullptr);
                imageAvailableSemaphores[i] = VK_NULL_HANDLE;
            }
            if (renderFinishedSemaphores[i] != VK_NULL_HANDLE) {
                vkDestroySemaphore(vkDevice, renderFinishedSemaphores[i], nullptr);
                renderFinishedSemaphores[i] = VK_NULL_HANDLE;
            }
            if (inFlightFences[i] != VK_NULL_HANDLE) {
                vkDestroyFence(vkDevice, inFlightFences[i], nullptr);
                inFlightFences[i] = VK_NULL_HANDLE;
            }
        }
    }

    void SyncObjects::createSyncObjects()
    {
        imageAvailableSemaphores.resize(maxFramesInFlight);
        renderFinishedSemaphores.resize(maxFramesInFlight);
        inFlightFences.resize(maxFramesInFlight);

        const VkDevice vkDevice = device.getDevice();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Start signaled to avoid waiting on the first frame

        for (size_t i = 0; i < maxFramesInFlight; ++i) {
            if (vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(vkDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create synchronization objects for a frame!");
            }
        }
    }

    const VkSemaphore& SyncObjects::getImageAvailableSemaphore(const uint32_t frameIndex) const
    {
        return imageAvailableSemaphores[frameIndex];
    }

    const VkSemaphore& SyncObjects::getRenderFinishedSemaphore(const uint32_t frameIndex) const
    {
        return renderFinishedSemaphores[frameIndex];
    }

    const VkFence& SyncObjects::getInFlightFence(const uint32_t frameIndex) const
    {
        return inFlightFences[frameIndex];
    }

    void SyncObjects::waitForInFlightFence(const uint32_t frameIndex) const
    {
	    const VkDevice vkDevice = device.getDevice();
        vkWaitForFences(vkDevice, 1, &inFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
    }

    void SyncObjects::resetInFlightFence(const uint32_t frameIndex) const
    {
	    const VkDevice vkDevice = device.getDevice();
        vkResetFences(vkDevice, 1, &inFlightFences[frameIndex]);
    }

} // namespace basalt
