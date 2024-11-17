#pragma once

#include <vulkan/vulkan.h>

#include "command_pool.h"

namespace basalt {

	class Device; // Forward declaration

    class Buffer {
    public:
        Buffer(Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Buffer();

        // Delete copy/move
        Buffer(Buffer&) = delete;
        Buffer(Buffer&&) = delete;
        Buffer& operator= (const Buffer&) = delete;
        Buffer&& operator= (const Buffer&&) = delete;

        // Update buffer data, handling different memory types appropriately
        void updateBuffer(const CommandPool& commandPool, const void* data, VkDeviceSize size, VkDeviceSize offset = 0) const;

        // Getters
        VkBuffer getBuffer() const { return buffer; }
        VkDeviceMemory getBufferMemory() const { return bufferMemory; }
        VkDeviceSize getSize() const { return bufferSize; }

    private:
        Device& device;
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        VkDeviceSize bufferSize;
        VkMemoryPropertyFlags memoryProperties; // Store memory properties used during allocation

        // Helper functions
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const CommandPool& commandPool) const;
    };

} // namespace basalt
