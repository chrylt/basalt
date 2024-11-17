#include "shader_module.h"

#include <fstream>
#include <stdexcept>

#include "device.h"

namespace basalt {

    ShaderModule::ShaderModule(Device& device, const std::string& filepath)
        : device(device), shaderModule(VK_NULL_HANDLE)
    {
        // Read SPIR-V code from file
        const std::vector<char> code = readFile(filepath);

        // Create the shader module
        createShaderModule(code);
    }

    ShaderModule::~ShaderModule()
    {
        if (shaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device.getDevice(), shaderModule, nullptr);
            shaderModule = VK_NULL_HANDLE;
        }
    }

    void ShaderModule::createShaderModule(const std::vector<char>& code)
    {
	    const VkDevice vkDevice = device.getDevice();

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    std::vector<char> ShaderModule::readFile(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        const size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

} // namespace basalt
