cmake_minimum_required(VERSION 3.16)
project(BasaltProject)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading external libraries
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# GLM (Math library)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLFW (Window management library)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# stb_image (Image loading library)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Assimp (3D resources loading library)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
)
FetchContent_MakeAvailable(assimp)

# Vulkan (Graphics API)
find_package(Vulkan REQUIRED)

# Add subdirectories
add_subdirectory(basalt)
add_subdirectory(shaders)
add_subdirectory(examples/hello_triangle)
