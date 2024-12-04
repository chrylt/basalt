#include "compute_pipeline.h"
#include "device.h"
#include "shader_module.h"
#include <stdexcept>

namespace basalt {

    ComputePipeline::ComputePipeline(Device& device, const std::string& computeShaderPath, const VkPipelineLayout pipelineLayout)
        : device(device)
    {
        // Create shader module
        ShaderModule computeShaderModule(device, computeShaderPath);

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        shaderStageInfo.module = computeShaderModule.getShaderModule();
        shaderStageInfo.pName = "main";

        VkComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.stage = shaderStageInfo;
        pipelineInfo.layout = pipelineLayout;

        if (vkCreateComputePipelines(device.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create compute pipeline!");
        }
    }

    ComputePipeline::~ComputePipeline()
    {
        vkDestroyPipeline(device.getDevice(), computePipeline, nullptr);
    }

} // namespace basalt
