//
// Created by loochis on 2/11/24.
//

#ifndef VOXCHIS_VKCCOMMANDMANAGER_H
#define VOXCHIS_VKCCOMMANDMANAGER_H


#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>
#include "Utils/VKCStructs.h"

using namespace std;

namespace VKChis {

    class VKCCommandManager {
    public:
        VkCommandPool commandPool;
        vector<VkCommandBuffer> commandBuffers;

        VKCCommandManager(uint32_t in_flags, VkDevice in_device, QueueFamilyIndices &in_indices, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result);
        ~VKCCommandManager();
    private:
        uint32_t flags;

        int MAX_FRAMES_IN_FLIGHT;

        VkDevice device;
        QueueFamilyIndices indices;
    };

} // VKChis

#endif //VOXCHIS_VKCCOMMANDMANAGER_H
