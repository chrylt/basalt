#include "surface.h"
#include "instance.h"

#include <stdexcept>

namespace basalt {

    Surface::Surface(Instance& instance, GLFWwindow* window)
        : instance(instance), surface(VK_NULL_HANDLE)
    {
        createSurface(window);
    }

    Surface::~Surface()
    {
        if (surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance.getInstance(), surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
    }

    void Surface::createSurface(GLFWwindow* window)
    {
        if (glfwCreateWindowSurface(instance.getInstance(), window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

} // namespace basalt
