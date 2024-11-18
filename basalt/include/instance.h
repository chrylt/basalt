#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Instance {
    public:
        Instance();
        ~Instance();

        // Delete copy/move
        Instance(Instance&) = delete;
        Instance(Instance&&) = delete;
        Instance& operator= (const Instance&) = delete;
        Instance&& operator= (const Instance&&) = delete;

        // Accessor
        VkInstance getInstance() const { return instance; }

        // Validation layers
        bool enableValidationLayers;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        // Required instance extensions
        std::vector<const char*> requiredExtensions;

    private:
        VkInstance instance;

        // Methods
        void createInstance();
        bool checkValidationLayerSupport() const;
        std::vector<const char*> getRequiredExtensions() const;
    };

} // namespace basalt
