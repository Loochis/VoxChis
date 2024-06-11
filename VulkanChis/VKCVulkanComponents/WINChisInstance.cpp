//
// Created by loochis on 1/28/24.
//

#include <stdexcept>
#include <iostream>
#include "WINChisInstance.h"
#include "../Headers/ColorMessages.h"
#include "../Headers/VKCEnumerations.h"

namespace VKChis {
    WINChisInstance::WINChisInstance(uint32_t in_flags, int width, int height, const std::string& title)
    :   flags(in_flags), width(width), height(height)
    {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Tell GLFW not to create OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // Enable resizing
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);     // Disable the bar on top

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (window == nullptr)  throw std::runtime_error("/// FATAL ERROR /// - Failed to create Window!");
        if (enableValidation)              print_colored("/// GOOD /// - Created Window", GREEN);
    }

    WINChisInstance::~WINChisInstance() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        glfwDestroyWindow(window);
        glfwTerminate();
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Window", CYAN);
    }
} // WINChis