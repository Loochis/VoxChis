//
// Created by loochis on 1/29/24.
//

#ifndef VOXCHIS_VKCHISSURFACE_H
#define VOXCHIS_VKCHISSURFACE_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace VKChis {

    class VKChisSurface {
    public:
        VkSurfaceKHR surface;
        VKChisSurface() = default;
        VKChisSurface(VkInstance &in_instance, GLFWwindow *in_window, VkResult &result);

        ~VKChisSurface();

    private:
        VkInstance instance;
        GLFWwindow* window;
    };

} // VKChis

#endif //VOXCHIS_VKCHISSURFACE_H
