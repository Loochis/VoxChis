//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_WINCHISINSTANCE_H
#define VOXCHIS_WINCHISINSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace VKChis {

    class WINChisInstance {
    public:
        GLFWwindow* window;
        int width;
        int height;

        WINChisInstance(uint32_t in_flags, int width, int height, const std::string& title);
        ~WINChisInstance();
    private:
        uint32_t flags;
    };

} // WINChis

#endif //VOXCHIS_WINCHISINSTANCE_H
