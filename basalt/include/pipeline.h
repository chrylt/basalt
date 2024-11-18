#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;       // Forward declaration
    class RenderPass;   // Forward declaration
    class SwapChain;    // Forward declaration

    class Pipeline {
    public:
        Pipeline(Device& device, RenderPass& renderPass, SwapChain& swapChain,
            const std::string& vertShaderPath, const std::string& fragShaderPath,
            VkVertexInputBindingDescription bindingDescription,
            const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);
        ~Pipeline();

        // Delete copy/move
        Pipeline(Pipeline&) = delete;
        Pipeline(Pipeline&&) = delete;
        Pipeline& operator= (const Pipeline&) = delete;
        Pipeline&& operator= (const Pipeline&&) = delete;

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
        void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath,
            VkVertexInputBindingDescription bindingDescription,
            const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);
    };

} // namespace basalt
