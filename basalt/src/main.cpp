#include <iostream>
#include <filesystem>

// glfw
#define GLFW_INCLUDE_VULKAN  // Include Vulkan headers in GLFW
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// stb_image inclusion
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// assimp
#include <assimp/Importer.hpp>   // Assimp's main include
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags

void loadModel(const std::string& filePath) {
    Assimp::Importer importer;

    // Load the model file
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << '\n';
        return;
    }

    // Print basic information about the loaded model
    std::cout << "Loaded model: " << filePath << '\n';
    std::cout << "Number of meshes: " << scene->mNumMeshes << '\n';
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
	    const aiMesh* mesh = scene->mMeshes[i];
        std::cout << "Mesh " << i << ": Vertices = " << mesh->mNumVertices << ", Faces = " << mesh->mNumFaces << '\n';
    }
}

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

    // ==================== Assimp Test ====================

    loadModel((baseResourcePath / "cube.obj").string());
    loadModel((baseResourcePath / "cube.fbx").string());

    // ==================== Main Loop ====================
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
