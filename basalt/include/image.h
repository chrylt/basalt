#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device; // Forward declaration

    class Image {
    public:
        Image(Device& device, uint32_t width, uint32_t height, VkFormat format,
            VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Image();

        // Delete copy/move
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        VkImage getImage() const { return image; }
        VkDeviceMemory getMemory() const { return imageMemory; }

    private:
        Device& device;
        VkImage image;
        VkDeviceMemory imageMemory;

    };

} // namespace basalt
