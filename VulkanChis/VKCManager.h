//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCMANAGER_H
#define VOXCHIS_VKCMANAGER_H

// VKChis includes
#include <memory>
#include "Utils/VKCEnumerations.h"
#include "WINChisInstance.h"
#include "VKCInstance.h"
#include "VKCSurface.h"
#include "VKCLogicalDevice.h"
#include "VKCSwapChain.h"
#include "VKCRenderPass.h"
#include "VKCShaderModule.h"

namespace VKChis {

    class VKCManager {
    private:

        const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        uint32_t flags;
        std::shared_ptr<WINChisInstance> window;
        std::unique_ptr<VKCInstance> instance;
        std::unique_ptr<VKCSurface> surface;
        std::unique_ptr<VKCLogicalDevice> device;
        std::unique_ptr<VKCSwapChain> swapChain;
        std::unique_ptr<VKCRenderPass> renderPass;
        std::vector<std::unique_ptr<VKCShaderModule>> shader_modules;

    public:
        VKCManager(std::shared_ptr<WINChisInstance> in_window, uint32_t in_flags);
        ~VKCManager();

    };

} // VKChis

#endif //VOXCHIS_VKCMANAGER_H
