//
// Created by loochis on 3/5/24.
//

#ifndef VOXCHIS_VKCDESCRIPTORSETLAYOUT_H
#define VOXCHIS_VKCDESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>
#include <memory>
#include "../VKCSubsystem/VKCDevice.h"

using namespace std;

namespace VKChis {

    class VKCDescriptorSetLayout {
    public:
        VKCDescriptorSetLayout(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, VkResult &result);
        ~VKCDescriptorSetLayout();

        VkDescriptorSetLayout descriptorSetLayout;
    private:
        uint32_t flags;

        shared_ptr<VKCDevice> device;
    };

} // VKChis

#endif //VOXCHIS_VKCDESCRIPTORSETLAYOUT_H
