//
// Created by loochis on 2/16/24.
//

#ifndef VOXCHIS_VKCBUFFER_H
#define VOXCHIS_VKCBUFFER_H

#include <vulkan/vulkan.h>
#include "../VKCSubsystem/VKCDevice.h"

using namespace std;

namespace VKChis {

    class VKCBuffer {
    public:
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;

        VKCBuffer(uint32_t in_flags,
                  shared_ptr<VKCDevice> &in_device,
                  VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkResult &result);

        ~VKCBuffer();
    private:
        uint32_t flags;
        shared_ptr<VKCDevice> device;

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };

} // VKChis

#endif //VOXCHIS_VKCBUFFER_H
