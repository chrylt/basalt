#include "library_test.h"
#include <iostream>
#include <filesystem>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void runLibraryTests(const std::string& resourcePath) {

    std::cout << "========= Running library tests =========" << '\n';

    // ==================== GLFW Test ====================
    std::cout << "Initializing GLFW..." << '\n';
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << '\n';
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << '\n';
        glfwTerminate();
        return;
    }
    std::cout << "GLFW initialized and window created successfully!" << '\n';

    // ==================== stb_image Test ====================
    std::cout << "Loading image with stb_image..." << '\n';
    const std::filesystem::path imagePath = resourcePath + "/test-texture.png";
    int imageWidth, imageHeight, imageChannels;

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
    constexpr glm::vec3 vectorA(1.0f, 2.0f, 3.0f);
    constexpr glm::vec3 vectorB(4.0f, 5.0f, 6.0f);
    constexpr glm::vec3 vectorSum = vectorA + vectorB;

    if constexpr (vectorSum == glm::vec3(5.0f, 7.0f, 9.0f)) {
        std::cout << "GLM vector addition test passed!" << '\n';
    }
    else {
        std::cerr << "GLM vector addition test failed!" << '\n';
    }

    // ==================== Assimp Test ====================
    auto loadModel = [](const std::string& filePath) -> void {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << '\n';
            return;
        }

        std::cout << "Loaded model: " << filePath << '\n';
        std::cout << "Number of meshes: " << scene->mNumMeshes << '\n';
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh* mesh = scene->mMeshes[i];
            std::cout << "Mesh " << i << ": Vertices = " << mesh->mNumVertices << ", Faces = " << mesh->mNumFaces << '\n';
        }
        };

    loadModel((std::filesystem::path(resourcePath) / "cube.obj").string());
    loadModel((std::filesystem::path(resourcePath) / "cube.fbx").string());

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "========= Library tests completed =========" << '\n';
}
