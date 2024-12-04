cmake_minimum_required(VERSION 3.16)
project(BasaltProject)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages that are already installed
find_package(glm REQUIRED CONFIG)
find_package(glfw3 REQUIRED)
find_package(stb REQUIRED)
find_package(assimp REQUIRED)
find_package(Vulkan REQUIRED)

# Add subdirectories
add_subdirectory(basalt)
add_subdirectory(shaders)
add_subdirectory(examples/hello_triangle)