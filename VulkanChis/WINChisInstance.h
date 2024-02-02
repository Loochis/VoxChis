//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_WINCHISINSTANCE_H
#define VOXCHIS_WINCHISINSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace WINChis {

    class WINChisInstance {
    public:
        GLFWwindow* window;
        WINChisInstance(int width, int height, const std::string& title);
    };

} // WINChis

#endif //VOXCHIS_WINCHISINSTANCE_H
