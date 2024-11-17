add_library(Basalt STATIC
    
    src/buffer.cpp
    src/command_pool.cpp
    src/device.cpp
    src/instance.cpp
    src/pipeline.cpp
    src/queue.cpp
    src/renderpass.cpp
    src/shader_module.cpp
    src/surface.cpp
    src/swapchain.cpp
    src/sync_objects.cpp
    src/utils.cpp
    "src/simple_vertex_2D.cpp"
 "include/command_buffer.h" "src/command_buffer.cpp")

target_include_directories(Basalt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${stb_SOURCE_DIR}
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Basalt PUBLIC Vulkan::Vulkan glfw glm assimp::assimp)

# Ensure the C++ standard is set
set_target_properties(Basalt PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
