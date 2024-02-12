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
#include "VKCGraphicsPipeline.h"
#include "VKCCommandManager.h"
#include "VKCSyncObjects.h"

using namespace std;

namespace VKChis {

    class VKCManager {
    private:

        // CONST vals,
        const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        const int MAX_FRAMES_IN_FLIGHT = 2;

        uint32_t flags;
        shared_ptr<WINChisInstance> window;
        unique_ptr<VKCInstance> instance;
        unique_ptr<VKCSurface> surface;
        unique_ptr<VKCLogicalDevice> device;
        unique_ptr<VKCSwapChain> swapChain;
        unique_ptr<VKCRenderPass> renderPass;

        shared_ptr<vector<VKCShaderModule>> shader_modules;
        unique_ptr<VKCGraphicsPipeline> graphicsPipeline;

        unique_ptr<VKCCommandManager> commandManager;
        shared_ptr<vector<VKCSyncObjects>> sync_objects;

        int currentFrame = 0;

        void RecordCommandBuffer(VkCommandBuffer commandBufferIn, uint32_t imageIndex);
        void RecreateSwapChain();
    public:
        VKCManager(std::shared_ptr<WINChisInstance> in_window, uint32_t in_flags);
        ~VKCManager();

        // Ohh boy this is where it gets complicated
        void DrawFrame();
    };

} // VKChis

#endif //VOXCHIS_VKCMANAGER_H
