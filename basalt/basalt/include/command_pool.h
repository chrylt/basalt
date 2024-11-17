#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device; // Forward declaration

    class CommandPool {
    public:
        CommandPool(Device& device, uint32_t queueFamilyIndex);
        ~CommandPool();

        // Delete copy/move
        CommandPool(CommandPool&) = delete;
        CommandPool(CommandPool&&) = delete;
        CommandPool& operator= (const CommandPool&) = delete;
        CommandPool&& operator= (const CommandPool&&) = delete;

        // Accessor
        VkCommandPool getCommandPool() const { return commandPool; }

        // Methods for single-time command buffer allocation and submission
        VkCommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue) const;

    private:
        Device& device;
        VkCommandPool commandPool;

        void createCommandPool(uint32_t queueFamilyIndex);
    };

} // namespace basalt
