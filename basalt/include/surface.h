#pragma once


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


namespace basalt {

    class Instance; // Forward declaration

    class Surface {
    public:
        Surface(Instance& instance, GLFWwindow* window);
        ~Surface();

        // Delete copy/move
        Surface(Surface&) = delete;
        Surface(Surface&&) = delete;
        Surface& operator= (const Surface&) = delete;
        Surface&& operator= (const Surface&&) = delete;

        // Accessor
        VkSurfaceKHR getSurface() const { return surface; }

    private:
        Instance& instance;
        VkSurfaceKHR surface;

        void createSurface(GLFWwindow* window);
    };

} // namespace basalt
