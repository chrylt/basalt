#pragma once

#include <vulkan/vulkan.h>
#include "device.h"
#include "command_pool.h"

namespace basalt {

    class CommandBuffer {
    public:
        CommandBuffer(Device& device, CommandPool& commandPool);
        ~CommandBuffer();

        // Delete copy/move
        CommandBuffer(CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&) = delete;
        CommandBuffer& operator= (const CommandBuffer&) = delete;
        CommandBuffer&& operator= (const CommandBuffer&&) = delete;

        VkCommandBuffer_T* const* get() const { return &commandBuffer; }

        void begin(VkCommandBufferUsageFlags flags = 0) const;
        void end() const;

        // Recording commands
        void beginRenderPass(VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent,
                             VkClearValue clearColor) const;
        void endRenderPass() const;
        void bindPipeline(VkPipeline pipeline) const;
        void bindVertexBuffer(VkBuffer vertexBuffer) const;
        void draw(uint32_t vertexCount) const;

    private:
        Device& device;
        CommandPool& commandPool;
        VkCommandBuffer commandBuffer;
    };

} // namespace basalt
