//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCHISINSTANCE_H
#define VOXCHIS_VKCHISINSTANCE_H


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
// VKCHIS INCLUDES
#include "WINChisInstance.h"
#include "VKCDebugMessenger.h"
#include "VKChisValidator.h"
#include "VKChisSurface.h"

namespace VKChis {

    class VKChisInstance {
    public:
        bool enableValidationLayers;
        VkInstance instance;
        WINChis::WINChisInstance *window;
        VKChisSurface surface;

        explicit VKChisInstance(WINChis::WINChisInstance &in_window, bool enableValidation);

        ~VKChisInstance();
    private:
        VKCDebugMessenger debugMessenger;

        void InitInstance(VkResult &result);
    };

} // VKChis

#endif //VOXCHIS_VKCHISINSTANCE_H
