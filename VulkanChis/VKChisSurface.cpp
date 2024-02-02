//
// Created by loochis on 1/29/24.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VKChisSurface.h"

namespace VKChis {
    VKChisSurface::VKChisSurface(VkInstance &in_instance, GLFWwindow &in_window, VkResult &result) {
        instance = in_instance;
        result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    }

    VKChisSurface::~VKChisSurface() = default;
} // VKChis