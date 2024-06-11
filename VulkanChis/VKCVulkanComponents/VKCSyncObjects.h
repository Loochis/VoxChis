//
// Created by loochis on 2/11/24.
//

#ifndef VOXCHIS_VKCSYNCOBJECTS_H
#define VOXCHIS_VKCSYNCOBJECTS_H

#include <cstdint>
#include <vulkan/vulkan.h>
#include "VKCDevice.h"

using namespace std;

namespace VKChis {

    class VKCSyncObjects {
    public:
        VkSemaphore renderFinishedSemaphore;
        VkSemaphore imageAvailableSemaphore;
        VkFence inFlightFence;

        VKCSyncObjects(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, VkResult &result);
        ~VKCSyncObjects();
    private:
        uint32_t flags;

        shared_ptr<VKCDevice> device;
    };

} // VKChis

#endif //VOXCHIS_VKCSYNCOBJECTS_H
