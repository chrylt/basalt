#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Set GLFW to use Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create a Vulkan-compatible GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Basalt - Vulkan", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}