//
// Created by loochis on 1/29/24.
//

#ifndef VOXCHIS_VKCEXTENSIONMANAGER_H
#define VOXCHIS_VKCEXTENSIONMANAGER_H

#include <vector>
#include <cstdint>
#include <GLFW/glfw3.h>

std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

#endif //VOXCHIS_VKCEXTENSIONMANAGER_H
