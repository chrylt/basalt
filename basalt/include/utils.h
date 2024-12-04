#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device;       // Forward declaration
    class CommandPool;  // Forward declaration
    class Queue;        // Forward declaration

    namespace utils {

        // Function to read a file into a byte buffer
        std::vector<char> readFile(const std::string& filename);

        // Function to find a suitable memory type
        uint32_t findMemoryType(Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

        // Function to copy buffer data
        void copyBuffer(Device& device, const CommandPool& commandPool, VkQueue queue,
            VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        // Function to transition image layouts
        void transitionImageLayout(Device& device, const CommandPool& commandPool, const Queue& queue,
            VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void transitionImageLayout(Device& device, const CommandPool& commandPool, const VkQueue& queue,
            VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);


        // Function to create an image view
        VkImageView createImageView(const Device& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    } // namespace utils

} // namespace basalt
