//
// Created by loochis on 2/5/24.
//

#ifndef VOXCHIS_VKCRENDERPASS_H
#define VOXCHIS_VKCRENDERPASS_H

#include <vulkan/vulkan.h>
#include <vector>
#include "VKCSubsystem/VKCSwapChain.h"

namespace VKChis {

    class VKCRenderPass {
    public:
        VkRenderPass renderPass;

        VKCRenderPass(uint32_t in_flags, VkDevice in_device, VkFormat in_swapChainImageFormat, VkResult &result);
        ~VKCRenderPass();
    private:
        uint32_t flags;

        VkDevice device;
        VkFormat swapChainImageFormat;
    };

} // VKChis

#endif //VOXCHIS_VKCRENDERPASS_H
