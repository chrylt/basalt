#pragma once

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;    // Forward declaration
    class SwapChain; // Forward declaration

    class RenderPass {
    public:
        RenderPass(Device& device, VkFormat swapChainImageFormat);
        ~RenderPass();

        // Delete copy/move
        RenderPass(RenderPass&) = delete;
        RenderPass(RenderPass&&) = delete;
        RenderPass& operator= (const RenderPass&) = delete;
        RenderPass&& operator= (const RenderPass&&) = delete;

        // Accessor
        VkRenderPass getRenderPass() const { return renderPass; }

    private:
        Device& device;
        VkRenderPass renderPass;

        void createRenderPass(VkFormat swapChainImageFormat);
    };

} // namespace basalt
