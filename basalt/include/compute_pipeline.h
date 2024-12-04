#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace basalt {

    class Device; // Forward declaration

    class ComputePipeline {
    public:
        ComputePipeline(Device& device, const std::string& computeShaderPath, VkPipelineLayout pipelineLayout);
        ~ComputePipeline();

        // Delete copy/move
        ComputePipeline(const ComputePipeline&) = delete;
        ComputePipeline& operator=(const ComputePipeline&) = delete;

        VkPipeline getPipeline() const { return computePipeline; }

    private:
        Device& device;
        VkPipeline computePipeline;
    };

} // namespace basalt
