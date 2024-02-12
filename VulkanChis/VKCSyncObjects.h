//
// Created by loochis on 2/11/24.
//

#ifndef VOXCHIS_VKCSYNCOBJECTS_H
#define VOXCHIS_VKCSYNCOBJECTS_H

#include <cstdint>
#include <vulkan/vulkan.h>

namespace VKChis {

    class VKCSyncObjects {
    public:
        VkSemaphore renderFinishedSemaphore;
        VkSemaphore imageAvailableSemaphore;
        VkFence inFlightFence;

        VKCSyncObjects(uint32_t in_flags, VkDevice in_device, VkResult &result);
        ~VKCSyncObjects();
    private:
        uint32_t flags;

        VkDevice device;
    };

} // VKChis

#endif //VOXCHIS_VKCSYNCOBJECTS_H
