#define GLFW_INCLUDE_VULKAN  // Include Vulkan headers in GLFW
#include <GLFW/glfw3.h>      // Include GLFW library
#include <iostream>
#include <filesystem>
#include <glm/glm.hpp>       // Include GLM library
#include <glm/gtc/matrix_transform.hpp>  // Include GLM transformations

// stb_image inclusion
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
    // ==================== GLFW Test ====================
    std::cout << "Initializing GLFW..." << '\n';

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << '\n';
        return -1;
    }

    // Set GLFW to use Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create a Vulkan-compatible GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Basalt - Vulkan", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << '\n';
        glfwTerminate();
        return -1;
    }

    std::cout << "GLFW initialized and window created successfully!" << '\n';

    // ==================== stb_image Test ====================
    std::cout << "Loading image with stb_image..." << '\n';

    // Assume the resources are stored in a folder called "resources" relative to the executable
    const std::filesystem::path baseResourcePath = std::filesystem::current_path() / ".." / ".." / ".." / "resources";

    // Test image loading using stb_image
    int imageWidth, imageHeight, imageChannels;

    // Construct the image path dynamically
    const std::filesystem::path imagePath = baseResourcePath / "test-texture.png";

    if (unsigned char* imageData = stbi_load(imagePath.string().c_str(), &imageWidth, &imageHeight, &imageChannels, 0)) {
        std::cout << "Image loaded successfully using stb_image!" << '\n';
        std::cout << "Image dimensions: " << imageWidth << "x" << imageHeight << ", Channels: " << imageChannels << '\n';
        stbi_image_free(imageData);  // Free image data after use
    }
    else {
        std::cerr << "Failed to load image using stb_image!" << '\n';
    }

    // ==================== GLM Test ====================
    std::cout << "Testing GLM library..." << '\n';

    // Create two glm vectors
    constexpr glm::vec3 vectorA(1.0f, 2.0f, 3.0f);
    constexpr glm::vec3 vectorB(4.0f, 5.0f, 6.0f);

    // Perform a vector addition
    constexpr glm::vec3 vectorSum = vectorA + vectorB;

    // Check if the result is correct
    if constexpr (vectorSum == glm::vec3(5.0f, 7.0f, 9.0f)) {
        std::cout << "GLM vector addition test passed!" << '\n';
    }
    else {
        std::cerr << "GLM vector addition test failed!" << '\n';
    }

    // ==================== Main Loop ====================
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
