#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace basalt {

    class Device; // Forward declaration

    class ShaderModule {
    public:
        ShaderModule(Device& device, const std::string& filepath);
        ~ShaderModule();

        // Delete copy/move
        ShaderModule(ShaderModule&) = delete;
        ShaderModule(ShaderModule&&) = delete;
        ShaderModule& operator= (const ShaderModule&) = delete;
        ShaderModule&& operator= (const ShaderModule&&) = delete;

        // Accessor
        VkShaderModule getShaderModule() const { return shaderModule; }

    private:
        Device& device;
        VkShaderModule shaderModule;

        // Helper methods
        void createShaderModule(const std::vector<char>& code);

        // Utility method
        static std::vector<char> readFile(const std::string& filepath);
    };

} // namespace basalt
