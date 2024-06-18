//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCMANAGER_H
#define VOXCHIS_VKCMANAGER_H

// VKChis includes
#include <memory>
#include "Headers/VKCEnumerations.h"
#include "VKCVulkanComponents/WINChisInstance.h"
#include "VKCVulkanComponents/VKCInstance.h"
#include "VKCVulkanComponents/VKCSurface.h"
#include "VKCVulkanComponents/VKCDevice.h"
#include "VKCVulkanComponents/VKCSwapChain.h"
#include "VKCVulkanComponents/VKCRenderPass.h"
#include "VKCVulkanComponents/VKCShaderModule.h"
#include "VKCVulkanComponents/VKCGraphicsPipeline.h"
#include "VKCVulkanComponents/VKCCommandManager.h"
#include "VKCVulkanComponents/VKCSyncObjects.h"
#include "VKCVulkanComponents/VKCBuffer.h"
#include "VKCVulkanComponents/VKCDescriptorManager.h"

#include "VKCUtils/VKCStatistics.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

// IMGUI
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_vulkan.h"
#include "VKCUtils/VKCAssetManager.h"
#include "VKCUtils/VKCUserInterface.h"

using namespace std;

namespace VKChis {

    class VKCManager {
    private:
        // TEMP
        glm::mat4 projMat;
        glm::mat4 viewMat;
        glm::mat4 obj1Mat;
        glm::mat4 obj2Mat;

        vector<glm::mat4> objMats;
        vector<glm::mat4> objMatsInv;
        int numObjs = 50;

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
        shared_ptr<VKCInstance> instance;
        shared_ptr<VKCSurface> surface;
        shared_ptr<VKCDevice> device; // SHARED
        shared_ptr<VKCSwapChain> swapChain;
        unique_ptr<VKCRenderPass> renderPass;

        shared_ptr<VKCDescriptorManager> descriptorManager;

        shared_ptr<vector<VKCShaderModule>> shader_modules;
        unique_ptr<VKCGraphicsPipeline> graphicsPipeline;

        shared_ptr<VKCCommandManager> commandManager;
        shared_ptr<vector<VKCSyncObjects>> sync_objects;

        unique_ptr<VKCBuffer> vert_buffer;
        unique_ptr<VKCBuffer> ind_buffer;

        unique_ptr<VKCUserInterface> userInterface;

        // imgui testing
        float testslider = 0;

        int currentFrame = 0;

        // statistics!
        // some bigger arrays here, store 'em on the heap
        unique_ptr<VKCStatistics> stats;

        // Asset management
        shared_ptr<VKCAssetManager> assetManager;

        void RecordCommandBuffer(VkCommandBuffer commandBufferIn, uint32_t imageIndex, ImDrawData* draw_data);
        void RecreateSwapChain();
    public:
        VKCManager(std::shared_ptr<WINChisInstance> &in_window, uint32_t in_flags);
        ~VKCManager();

        // Ohh boy this is where it gets complicated
        void DrawFrame();
        void updateUniformBuffer(uint32_t currentImage);
    };

} // VKChis

#endif //VOXCHIS_VKCMANAGER_H
