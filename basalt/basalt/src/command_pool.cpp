#include "command_pool.h"

#include <memory>
#include <stdexcept>

#include "command_buffer.h"
#include "device.h"

namespace basalt {

    CommandPool::CommandPool(Device& device, const uint32_t queueFamilyIndex)
        : device(device), commandPool(VK_NULL_HANDLE)
    {
        createCommandPool(queueFamilyIndex);
    }

    CommandPool::~CommandPool()
    {
        if (commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device.getDevice(), commandPool, nullptr);
            commandPool = VK_NULL_HANDLE;
        }
    }

    void CommandPool::createCommandPool(const uint32_t queueFamilyIndex)
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndex;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device.getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    VkCommandBuffer CommandPool::beginSingleTimeCommands() const
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffer!");
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void CommandPool::endSingleTimeCommands(const VkCommandBuffer commandBuffer, const VkQueue queue) const
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit command buffer!");
        }

        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(device.getDevice(), commandPool, 1, &commandBuffer);
    }

} // namespace basalt
