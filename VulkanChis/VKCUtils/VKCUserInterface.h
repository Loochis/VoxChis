//
// Created by loochis on 6/11/24.
//

#ifndef VOXCHIS_VKCUSERINTERFACE_H
#define VOXCHIS_VKCUSERINTERFACE_H


#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_vulkan.h"

#include "../VKCVulkanComponents/VKCInstance.h"
#include "../VKCVulkanComponents/VKCSurface.h"
#include "../VKCVulkanComponents/VKCDevice.h"
#include "../VKCVulkanComponents/VKCSwapChain.h"
#include "../VKCVulkanComponents/WINChisInstance.h"
#include "../VKCVulkanComponents/VKCDescriptorManager.h"
#include "../VKCVulkanComponents/VKCRenderPass.h"

using namespace VKChis;

class VKCUserInterface {
public:
    int imgui_imgcount = 2;
    ImGui_ImplVulkanH_Window imgui_wd;

    VKCUserInterface(shared_ptr<VKCDevice> &in_device, shared_ptr<VKCInstance> &in_instance,
                     shared_ptr<VKCSwapChain> &in_swapchain, shared_ptr<VKCSurface> &in_surface, shared_ptr<WINChisInstance> &in_window,
                     shared_ptr<VKCRenderPass> &in_renderPass, shared_ptr<VKCDescriptorManager> &in_descriptorManager);

    ~VKCUserInterface();

private:
    shared_ptr<VKCDevice> device;
    shared_ptr<VKCInstance> instance;
    shared_ptr<VKCSwapChain> swapchain;
    shared_ptr<VKCSurface> surface;
    shared_ptr<WINChisInstance> window;
    shared_ptr<VKCRenderPass> renderPass;
    shared_ptr<VKCDescriptorManager> descriptorManager;
};


#endif //VOXCHIS_VKCUSERINTERFACE_H
