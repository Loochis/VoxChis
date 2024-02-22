//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCINSTANCE_H
#define VOXCHIS_VKCINSTANCE_H


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
// VKCHIS INCLUDES
#include "../Utils/VKCEnumerations.h"
#include "VKCValidator.h"
#include "VKCDebugMessenger.h"
#include <memory>

namespace VKChis {

    class VKCInstance {
    public:
        VkInstance instance;

        VKCInstance(uint32_t in_flags, const std::vector<const char*> &validationLayers, VkResult &result);
        ~VKCInstance();

    private:
        uint32_t flags;

        std::unique_ptr<VKCDebugMessenger> debugMessenger;
        static std::vector<const char*> getRequiredExtensions();
    };

} // VKChis

#endif //VOXCHIS_VKCINSTANCE_H
