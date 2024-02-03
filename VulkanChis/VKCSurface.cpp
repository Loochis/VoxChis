//
// Created by loochis on 1/29/24.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "VKCSurface.h"

namespace VKChis {
    VKCSurface::VKCSurface(VkInstance in_instance, GLFWwindow *window, VkResult &result) : instance(in_instance) {
        result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    }

    VKCSurface::~VKCSurface() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        std::cout << "/// CLEAN /// - Destroyed Surface" << std::endl;
    }
} // VKChis