#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace basalt {

    class Device; // Forward declaration

    class DescriptorPool {
    public:
        DescriptorPool(Device& device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~DescriptorPool();

        // Delete copy/move
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        VkDescriptorPool getPool() const { return descriptorPool; }

    private:
        Device& device;
        VkDescriptorPool descriptorPool;
    };

} // namespace basalt
