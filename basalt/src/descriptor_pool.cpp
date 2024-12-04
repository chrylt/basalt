#include "descriptor_pool.h"
#include "device.h"
#include <stdexcept>

namespace basalt {

    DescriptorPool::DescriptorPool(Device& device, const uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes)
        : device(device)
    {
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;

        if (vkCreateDescriptorPool(device.getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool!");
        }
    }

    DescriptorPool::~DescriptorPool()
    {
        vkDestroyDescriptorPool(device.getDevice(), descriptorPool, nullptr);
    }

} // namespace basalt
