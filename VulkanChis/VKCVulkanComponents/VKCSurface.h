//
// Created by loochis on 1/29/24.
//

#ifndef VOXCHIS_VKCSURFACE_H
#define VOXCHIS_VKCSURFACE_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace VKChis {

    class VKCSurface {
    public:
        VkSurfaceKHR surface;
        VKCSurface(uint32_t in_flags, VkInstance in_instance, GLFWwindow *window, VkResult &result);

        ~VKCSurface();
    private:
        uint32_t flags;

        VkInstance instance;
    };

} // VKChis

#endif //VOXCHIS_VKCSURFACE_H
