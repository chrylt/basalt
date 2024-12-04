#pragma once

#include <optional>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "queue.h"

namespace basalt {

    class Instance; // Forward declaration
    class Surface;  // Forward declaration

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> transfer_family;

        bool isComplete() const {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    class Device {
    public:
        Device(Instance& instance, Surface& surface);
        ~Device();

        // Delete copy/move
        Device(Device&) = delete;
        Device(Device&&) = delete;
        Device& operator= (const Device&) = delete;
        Device&& operator= (const Device&&) = delete;

        // Accessors
        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkQueue getPresentQueue() const { return presentQueue; }
        VkQueue getTransferQueue() const { return transferQueue != VK_NULL_HANDLE ? transferQueue : graphicsQueue; }
        uint32_t getGraphicsQueueFamilyIndex() const { return queueFamilyIndices.graphics_family.value(); }
        uint32_t getPresentQueueFamilyIndex() const { return queueFamilyIndices.present_family.value(); }
        uint32_t getTransferQueueFamilyIndex() const { return queueFamilyIndices.transfer_family.value(); }

        // Helper methods
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        // Memory type finder
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        // Rendering methods
        VkResult submitCommandBuffers(const VkCommandBuffer* commandBuffers, uint32_t commandBufferCount,
            const VkSemaphore* waitSemaphores, uint32_t waitSemaphoreCount,
            const VkPipelineStageFlags* waitStages,
            const VkSemaphore* signalSemaphores, uint32_t signalSemaphoreCount,
            VkFence fence) const;

        // Get Properties
        const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& getRayTracingProperties() const {
            return rayTracingPipelineProperties;
        }

    private:
        // Members
        Instance& instance;
        Surface& surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;

        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkQueue transferQueue = VK_NULL_HANDLE; // New queue for transfers

        QueueFamilyIndices queueFamilyIndices;

        VkPhysicalDeviceMemoryProperties memoryProperties;
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelineProperties{};

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
		    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		    VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		    VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		    VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME
        };

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelStructFeatures{};
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures{};
        VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
        VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures{};

        // Methods
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device) const;
        bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
        void queryRayTracingProperties();
    };

} // namespace basalt
