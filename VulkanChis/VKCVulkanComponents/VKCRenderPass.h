//
// Created by loochis on 2/5/24.
//

#ifndef VOXCHIS_VKCRENDERPASS_H
#define VOXCHIS_VKCRENDERPASS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "VKCSwapChain.h"
#include "VKCDevice.h"
#include "VKCImage.h"

namespace VKChis {

    using namespace std;

    class VKCRenderPass {
    public:
        VkRenderPass renderPass;

        VKCRenderPass(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCSwapChain> &in_swapChain, VkResult &result);
        ~VKCRenderPass();
    private:
        uint32_t flags;

        shared_ptr<VKCDevice> device;
        shared_ptr<VKCSwapChain> swapChain;
    };

} // VKChis

#endif //VOXCHIS_VKCRENDERPASS_H
