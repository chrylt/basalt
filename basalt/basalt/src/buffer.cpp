#include "buffer.h"

#include <stdexcept>

#include "command_pool.h"
#include "device.h"

namespace basalt {

    Buffer::Buffer(Device& device, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties)
        : device(device), buffer(VK_NULL_HANDLE), bufferMemory(VK_NULL_HANDLE), bufferSize(size), memoryProperties(properties)
    {
        createBuffer(size, usage, properties);
    }

    Buffer::~Buffer()
    {
        if (buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(device.getDevice(), buffer, nullptr);
        }
        if (bufferMemory != VK_NULL_HANDLE) {
            vkFreeMemory(device.getDevice(), bufferMemory, nullptr);
        }
    }

    void Buffer::updateBuffer(const CommandPool& commandPool, const void* data, const VkDeviceSize size, const VkDeviceSize offset) const
    {
        // Check if buffer memory is host-visible
        if (memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            // Directly map and copy the data
            void* mappedData;
            const VkResult result = vkMapMemory(device.getDevice(), bufferMemory, offset, size, 0, &mappedData);
            if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to map buffer memory!");
            }
            std::memcpy(mappedData, data, static_cast<size_t>(size));
            vkUnmapMemory(device.getDevice(), bufferMemory);
        }
        else {
            // Use a staging buffer to transfer the data

            // Create a staging buffer
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            constexpr VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            constexpr VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

            // Create staging buffer
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = stagingUsage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to create staging buffer!");
            }

            // Allocate memory for staging buffer
            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device.getDevice(), stagingBuffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, stagingProperties);

            if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate staging buffer memory!");
            }

            vkBindBufferMemory(device.getDevice(), stagingBuffer, stagingBufferMemory, 0);

            // Map memory and copy data
            void* mappedData;
            vkMapMemory(device.getDevice(), stagingBufferMemory, 0, size, 0, &mappedData);
            std::memcpy(mappedData, data, static_cast<size_t>(size));
            vkUnmapMemory(device.getDevice(), stagingBufferMemory);

            // Copy from staging buffer to device buffer
            copyBuffer(stagingBuffer, buffer, size, commandPool);

            // Cleanup staging resources
            vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
            vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
        }
    }

    void Buffer::createBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties)
    {
        // Create buffer
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT; // Ensure buffer can be a transfer destination if needed
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        // Get memory requirements
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device.getDevice(), buffer, &memRequirements);

        // Allocate memory
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device.getDevice(), buffer, bufferMemory, 0);
    }

    void Buffer::copyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize size, const CommandPool& commandPool) const
    {
	    const VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands();

        VkBufferCopy copyRegion;
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        commandPool.endSingleTimeCommands(commandBuffer, device.getGraphicsQueue()); // TODO: change to transfer queue when implemented
    }


} // namespace basalt
