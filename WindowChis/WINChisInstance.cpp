//
// Created by loochis on 1/28/24.
//

#include "WINChisInstance.h"

namespace WINChis {
    WINChisInstance::WINChisInstance(int width, int height, const std::string& title) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Tell GLFW not to create OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);     // Enable resizing

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }
} // WINChis