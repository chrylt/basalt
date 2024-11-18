#include "command_buffer.h"
#include <stdexcept>

namespace basalt {

    CommandBuffer::CommandBuffer(Device& device, CommandPool& commandPool)
        : device(device), commandPool(commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool.getCommandPool();
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffer!");
        }
    }

    CommandBuffer::~CommandBuffer()
    {
        vkFreeCommandBuffers(device.getDevice(), commandPool.getCommandPool(), 1, &commandBuffer);
    }

    void CommandBuffer::begin(const VkCommandBufferUsageFlags flags) const
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }
    }

    void CommandBuffer::end() const
    {
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void CommandBuffer::beginRenderPass(const VkRenderPass renderPass, const VkFramebuffer framebuffer, const VkExtent2D extent,
                                        const VkClearValue clearColor) const
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::endRenderPass() const
    {
        vkCmdEndRenderPass(commandBuffer);
    }

    void CommandBuffer::bindPipeline(const VkPipeline pipeline) const
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    void CommandBuffer::bindVertexBuffer(const VkBuffer vertexBuffer) const
    {
	    constexpr VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);
    }

    void CommandBuffer::draw(const uint32_t vertexCount) const
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

} // namespace basalt
