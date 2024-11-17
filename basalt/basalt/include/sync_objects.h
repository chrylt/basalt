#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device; // Forward declaration

    class SyncObjects {
    public:
        SyncObjects(Device& device, uint32_t maxFramesInFlight);
        ~SyncObjects();

        // Delete copy/move
        SyncObjects(SyncObjects&) = delete;
        SyncObjects(SyncObjects&&) = delete;
        SyncObjects& operator= (const SyncObjects&) = delete;
        SyncObjects&& operator= (const SyncObjects&&) = delete;

        // Accessors
        const VkSemaphore& getImageAvailableSemaphore(uint32_t frameIndex) const;
        const VkSemaphore& getRenderFinishedSemaphore(uint32_t frameIndex) const;
        const VkFence& getInFlightFence(uint32_t frameIndex) const;
        uint32_t getMaxFramesInFlight() const { return maxFramesInFlight; }

        // Methods
        void waitForInFlightFence(uint32_t frameIndex) const;
        void resetInFlightFence(uint32_t frameIndex) const;

    private:
        Device& device;
        uint32_t maxFramesInFlight;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        void createSyncObjects();
    };

} // namespace basalt
