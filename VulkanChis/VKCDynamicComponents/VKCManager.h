//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCMANAGER_H
#define VOXCHIS_VKCMANAGER_H

// VKChis includes
#include <memory>
#include "../Utils/VKCEnumerations.h"
#include "WINChisInstance.h"
#include "../VKCSubsystem/VKCInstance.h"
#include "../VKCSubsystem/VKCSurface.h"
#include "../VKCSubsystem/VKCDevice.h"
#include "../VKCSubsystem/VKCSwapChain.h"
#include "VKCRenderPass.h"
#include "VKCShaderModule.h"
#include "VKCGraphicsPipeline.h"
#include "../VKCSubsystem/VKCCommandManager.h"
#include "VKCSyncObjects.h"
#include "VKCBuffer.h"
#include "../VKCSubsystem/VKCDescriptorManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

// IMGUI
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_vulkan.h"

using namespace std;

namespace VKChis {

    class VKCManager {
    private:
        // TEMP
        glm::mat4 projMat;
        glm::mat4 viewMat;
        glm::mat4 obj1Mat;
        glm::mat4 obj2Mat;

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
        shared_ptr<VKCDevice> device; // SHARED
        unique_ptr<VKCSwapChain> swapChain;
        unique_ptr<VKCRenderPass> renderPass;

        shared_ptr<VKCDescriptorManager> descriptorManager;

        shared_ptr<vector<VKCShaderModule>> shader_modules;
        unique_ptr<VKCGraphicsPipeline> graphicsPipeline;

        unique_ptr<VKCCommandManager> commandManager;
        shared_ptr<vector<VKCSyncObjects>> sync_objects;

        unique_ptr<VKCBuffer> vert_buffer;
        unique_ptr<VKCBuffer> ind_buffer;

        // imgui testing
        ImGui_ImplVulkanH_Window imgui_wd;
        int                      imgui_imgcount = 2;
        float testslider = 0;

        int currentFrame = 0;

        void RecordCommandBuffer(VkCommandBuffer commandBufferIn, uint32_t imageIndex, ImDrawData* draw_data);
        void RecreateSwapChain();
    public:
        VKCManager(std::shared_ptr<WINChisInstance> in_window, uint32_t in_flags);
        ~VKCManager();

        // Ohh boy this is where it gets complicated
        void DrawFrame();
        void updateUniformBuffer(uint32_t currentImage);
    };

} // VKChis

#endif //VOXCHIS_VKCMANAGER_H
