//
// Created by loochis on 2/11/24.
//

#ifndef VOXCHIS_VKCCOMMANDMANAGER_H
#define VOXCHIS_VKCCOMMANDMANAGER_H


#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>
#include "../Headers/VKCStructs.h"
#include "VKCDevice.h"

using namespace std;

namespace VKChis {

    class VKCCommandManager {
    public:
        VkCommandPool commandPool;
        vector<VkCommandBuffer> commandBuffers;

        void CMD_CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        VKCCommandManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result);
        ~VKCCommandManager();
    private:
        uint32_t flags;

        int MAX_FRAMES_IN_FLIGHT;

        shared_ptr<VKCDevice> device;
        QueueFamilyIndices indices;
    };

} // VKChis

#endif //VOXCHIS_VKCCOMMANDMANAGER_H
