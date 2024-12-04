#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace basalt {

    class Device; // Forward declaration

    class DescriptorSetLayout {
    public:
        DescriptorSetLayout(Device& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
        ~DescriptorSetLayout();

        // Delete copy/move
        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getLayout() const { return descriptorSetLayout; }

    private:
        Device& device;
        VkDescriptorSetLayout descriptorSetLayout;
    };

} // namespace basalt
