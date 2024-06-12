//
// Created by loochis on 2/16/24.
//

#ifndef VOXCHIS_VKCBUFFER_H
#define VOXCHIS_VKCBUFFER_H

#include <vulkan/vulkan.h>
#include "VKCDevice.h"

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

        static uint32_t findMemoryType(shared_ptr<VKCDevice> &in_device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    private:
        uint32_t flags;
        shared_ptr<VKCDevice> device;
    };

} // VKChis

#endif //VOXCHIS_VKCBUFFER_H
