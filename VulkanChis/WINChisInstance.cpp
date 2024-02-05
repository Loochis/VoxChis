//
// Created by loochis on 1/28/24.
//

#include <stdexcept>
#include <iostream>
#include "WINChisInstance.h"
#include "Utils/ColorMessages.h"

namespace VKChis {
    WINChisInstance::WINChisInstance(int width, int height, const std::string& title) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Tell GLFW not to create OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);     // Enable resizing

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (window == nullptr)  throw std::runtime_error("/// FATAL ERROR /// - Failed to create Window!");
        else                    print_colored("/// GOOD /// - Created Window", GREEN);
    }

    WINChisInstance::~WINChisInstance() {
        glfwDestroyWindow(window);
        glfwTerminate();
        print_colored("/// CLEAN /// - Destroyed Window", CYAN);
    }
} // WINChis