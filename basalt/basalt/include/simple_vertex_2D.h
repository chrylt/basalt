#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace basalt {

    struct SimpleVertex2D {
        glm::vec2 pos;
        glm::vec3 color;

        // Methods to get Vulkan binding and attribute descriptions
        static VkVertexInputBindingDescription getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        // Equality operator (optional, useful for certain cases)
        bool operator==(const SimpleVertex2D& other) const {
            return pos == other.pos && color == other.color;
        }
    };

} // namespace basalt
