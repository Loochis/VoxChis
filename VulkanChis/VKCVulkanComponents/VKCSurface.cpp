//
// Created by loochis on 1/29/24.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "VKCSurface.h"
#include "../Headers/ColorMessages.h"
#include "../Headers/VKCEnumerations.h"

namespace VKChis {
    VKCSurface::VKCSurface(uint32_t in_flags, VkInstance in_instance, GLFWwindow *window, VkResult &result)
    : instance(in_instance),
    flags(in_flags)
    {
        result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    }

    VKCSurface::~VKCSurface() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        //vkDestroySurfaceKHR(instance, surface, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Surface", CYAN);
    }
} // VKChis