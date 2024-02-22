//
// Created by loochis on 2/16/24.
//

#ifndef VOXCHIS_VKCBUFFER_H
#define VOXCHIS_VKCBUFFER_H

#include <vulkan/vulkan.h>

namespace VKChis {

    class VKCBuffer {
    public:
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;

        VKCBuffer(uint32_t in_flags,
                  VkDevice in_device,
                  VkPhysicalDevice in_physDevice,
                  VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkResult &result);

        ~VKCBuffer();
    private:
        uint32_t flags;
        VkDevice device;
        VkPhysicalDevice physDevice;

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };

} // VKChis

#endif //VOXCHIS_VKCBUFFER_H
