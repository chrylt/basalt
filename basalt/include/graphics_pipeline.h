#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;       // Forward declaration
    class RenderPass;   // Forward declaration
    class SwapChain;    // Forward declaration

    class GraphicsPipeline {
    public:
        GraphicsPipeline(Device& device, RenderPass& renderPass, SwapChain& swapChain,
            const std::string& vertShaderPath, const std::string& fragShaderPath,
            const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
            VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE);
        ~GraphicsPipeline();

        // Delete copy/move
        GraphicsPipeline(GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        GraphicsPipeline& operator= (const GraphicsPipeline&) = delete;
        GraphicsPipeline&& operator= (const GraphicsPipeline&&) = delete;

        // Accessor
        VkPipeline getPipeline() const { return graphicsPipeline; }
        VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }

    private:
        // Members
        Device& device;
        RenderPass& renderPass;
        SwapChain& swapChain;

        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;

        // Methods
        void createGraphicsPipeline(
            const std::string& vertShaderPath,
            const std::string& fragShaderPath,
            const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
            VkDescriptorSetLayout descriptorSetLayout);
    };

} // namespace basalt
