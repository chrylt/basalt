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
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# GLFW (Window management library)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# stb_image (Image loading library)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Vulkan (Graphics API)
find_package(Vulkan REQUIRED)

# Define the executable
add_executable(Basalt src/main.cpp)

# Include directories
target_include_directories(Basalt PUBLIC ${stb_SOURCE_DIR} ${Vulkan_INCLUDE_DIR})

# Link libraries
target_link_libraries(Basalt PUBLIC glm glfw Vulkan::Vulkan)

