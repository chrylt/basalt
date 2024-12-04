#include "descriptor_set_layout.h"
#include "device.h"
#include <stdexcept>

namespace basalt {

    DescriptorSetLayout::DescriptorSetLayout(Device& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings)
        : device(device)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout!");
        }
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(device.getDevice(), descriptorSetLayout, nullptr);
    }

} // namespace basalt
