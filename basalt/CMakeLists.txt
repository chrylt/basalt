cmake_minimum_required(VERSION 3.16)
project(Basalt)

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

# Add the shaders subdirectory for automatic compilation
add_subdirectory(shaders)

# Define the executable after all dependencies are set up
add_executable(Basalt src/main.cpp src/library_test.cpp src/stb_image_impl.cpp)

# Include directories
target_include_directories(Basalt PUBLIC ${stb_SOURCE_DIR} ${Vulkan_INCLUDE_DIR})

# Link libraries
target_link_libraries(Basalt PUBLIC glm glfw Vulkan::Vulkan assimp::assimp)

# Ensure the Shaders target is built before the Basalt executable
add_dependencies(Basalt Shaders)

# Ensure SPIR-V files are treated as source files for Basalt
target_sources(Basalt PRIVATE ${SPIRV_FILES})


