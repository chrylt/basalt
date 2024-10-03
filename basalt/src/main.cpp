#include <filesystem>
#include <iostream>
#include "library_test.h"
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

// Uncomment this line to enable tests
// #define RUN_TESTS

void createVulkanInstance(VkInstance& instance) {
    // Application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "My Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    // Get required GLFW extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Instance Create Info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;

    // Create the Vulkan Instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

int main() {
	const std::filesystem::path baseResourcePath = std::filesystem::current_path() / ".." / ".." / ".." / "resources";
    
#ifdef RUN_TESTS
	runLibraryTests(baseResourcePath.string());
#endif

    // Initialize GLFW and create a window (ensure GLFW supports Vulkan)
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

    // Create Vulkan Instance
    VkInstance instance;
    createVulkanInstance(instance);

    std::cout << "Vulkan instance created successfully!" << '\n';

    // Clean up
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}