#include "device.h"

#include <set>
#include <stdexcept>

#include "instance.h"
#include "surface.h"

namespace basalt {

    Device::Device(Instance& instance, Surface& surface)
        : instance(instance), surface(surface)
    {
        pickPhysicalDevice();
        createLogicalDevice();
    }

    Device::~Device()
    {
        if (device != VK_NULL_HANDLE) {
            vkDestroyDevice(device, nullptr);
            device = VK_NULL_HANDLE;
        }
    }

    void Device::pickPhysicalDevice()
    {
        const VkInstance vkInstance = instance.getInstance();

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }

        // Get memory properties after selecting the physical device
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    }

    void Device::createLogicalDevice()
    {
        queueFamilyIndices = findQueueFamilies(physicalDevice);

        // Optional: Find a dedicated transfer queue family
        uint32_t transferQueueFamilyIndex = -1;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        // Find a queue family that supports transfer operations and is not graphics
        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            if ((queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                !(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                transferQueueFamilyIndex = i;
                break;
            }
        }

        if (transferQueueFamilyIndex == -1) {
            // Fallback to graphics queue if no dedicated transfer queue is found
            queueFamilyIndices.transfer_family = queueFamilyIndices.graphics_family.value();
        }

        // Store the transfer queue family index
        queueFamilyIndices.transfer_family = transferQueueFamilyIndex;

        // Collect unique queue families to create
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            queueFamilyIndices.graphics_family.value(),
            queueFamilyIndices.present_family.value(),
            queueFamilyIndices.transfer_family.value()
        };

        constexpr float queuePriority = 1.0f;
        for (const uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Initialize feature structures
        accelStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        bufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;

        // Chain the features together
        bufferDeviceAddressFeatures.pNext = nullptr;
        descriptorIndexingFeatures.pNext = &bufferDeviceAddressFeatures;
        rayTracingPipelineFeatures.pNext = &descriptorIndexingFeatures;
        accelStructFeatures.pNext = &rayTracingPipelineFeatures;

        VkPhysicalDeviceFeatures2 deviceFeatures2{};
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &accelStructFeatures;

        // Query the features
        vkGetPhysicalDeviceFeatures2(physicalDevice, &deviceFeatures2);

        // Check if required features are supported
        if (!accelStructFeatures.accelerationStructure || !rayTracingPipelineFeatures.rayTracingPipeline) {
            throw std::runtime_error("Ray tracing not supported on this device.");
        }

        // Prepare device create info
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = &deviceFeatures2;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.pEnabledFeatures = nullptr; // Must be null when using VkPhysicalDeviceFeatures2


        // Enable validation layers (deprecated, but required on some platforms)
        if (instance.enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(instance.validationLayers.size());
            createInfo.ppEnabledLayerNames = instance.validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }

        // Query ray tracing properties after device creation
        queryRayTracingProperties();

        // Retrieve queues
        vkGetDeviceQueue(device, queueFamilyIndices.graphics_family.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.present_family.value(), 0, &presentQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.transfer_family.value(), 0, &transferQueue);
    }

    void Device::queryRayTracingProperties() {
        // Initialize ray tracing pipeline properties
        rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
        VkPhysicalDeviceProperties2 deviceProperties2{};
        deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        deviceProperties2.pNext = &rayTracingPipelineProperties;
        vkGetPhysicalDeviceProperties2(physicalDevice, &deviceProperties2);
    }

    QueueFamilyIndices Device::findQueueFamilies(const VkPhysicalDevice device) const
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0) {
            throw std::runtime_error("Failed to find any queue families!");
        }

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int index = 0;
        for (const auto& queueFamily : queueFamilies) {
            // Check for graphics support
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = index;
            }

            // Check for presentation support
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface.getSurface(), &presentSupport);

            if (presentSupport) {
                indices.present_family = index;
            }

            if (indices.isComplete()) {
                break;
            }

            index++;
        }

        return indices;
    }

    bool Device::isDeviceSuitable(const VkPhysicalDevice device) const
    {
        const QueueFamilyIndices indices = findQueueFamilies(device);

        const bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            // Query swap chain support details (omitted here, assume adequate)
            swapChainAdequate = true;
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    bool Device::checkDeviceExtensionSupport(const VkPhysicalDevice device) const
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        if (extensionCount == 0) {
            return false;
        }

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    uint32_t Device::findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
    {
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    VkResult Device::submitCommandBuffers(const VkCommandBuffer* commandBuffers, const uint32_t commandBufferCount,
        const VkSemaphore* waitSemaphores, const uint32_t waitSemaphoreCount,
        const VkPipelineStageFlags* waitStages,
        const VkSemaphore* signalSemaphores, const uint32_t signalSemaphoreCount,
        const VkFence fence) const
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = waitSemaphoreCount;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = commandBufferCount;
        submitInfo.pCommandBuffers = commandBuffers;
        submitInfo.signalSemaphoreCount = signalSemaphoreCount;
        submitInfo.pSignalSemaphores = signalSemaphores;

        return vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);
    }

} // namespace basalt
