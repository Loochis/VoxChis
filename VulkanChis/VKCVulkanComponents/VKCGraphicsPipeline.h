//
// Created by loochis on 2/5/24.
//

#ifndef VOXCHIS_VKCGRAPHICSPIPELINE_H
#define VOXCHIS_VKCGRAPHICSPIPELINE_H

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>
#include <memory>

#include "VKCShaderModule.h"
#include "VKCDescriptorManager.h"

using namespace std;

namespace VKChis {

    class VKCGraphicsPipeline {
    public:
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        explicit VKCGraphicsPipeline(uint32_t in_flags, shared_ptr<vector<VKCShaderModule>> &in_shader_modules, VkExtent2D in_swapChainExtent, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCDescriptorManager> &in_descSetManager, VkRenderPass in_renderPass, VkResult &result);
        ~VKCGraphicsPipeline();

    private:
        uint32_t flags;

        VkExtent2D swapChainExtent;
        shared_ptr<vector<VKCShaderModule>> shader_modules;
        shared_ptr<VKCDevice> device;
        shared_ptr<VKCDescriptorManager> descSetManager;
        VkRenderPass renderPass;
    };

} // VKChis

#endif //VOXCHIS_VKCGRAPHICSPIPELINE_H
