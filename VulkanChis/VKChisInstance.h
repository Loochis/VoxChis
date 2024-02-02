//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCHISINSTANCE_H
#define VOXCHIS_VKCHISINSTANCE_H


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
// VKCHIS INCLUDES
#include "VKCDebugMessenger.h"
#include "VKChisValidator.h"
#include "VKChisSurface.h"

namespace VKChis {

    class VKChisInstance {
    public:
        bool enableValidationLayers;
        VkInstance instance;
        GLFWwindow *window;
        VKChisSurface *surface;

        explicit VKChisInstance(GLFWwindow &in_window, bool enableValidation);
    private:
        VKCDebugMessenger debugMessenger;

        void InitInstance(VkResult &result);
    };

} // VKChis

#endif //VOXCHIS_VKCHISINSTANCE_H
