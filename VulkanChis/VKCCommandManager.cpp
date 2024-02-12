//
// Created by loochis on 2/11/24.
//

#include "VKCCommandManager.h"
#include "Utils/ColorMessages.h"
#include "Utils/VKCEnumerations.h"

namespace VKChis {
    VKCCommandManager::VKCCommandManager(uint32_t in_flags, VkDevice in_device, QueueFamilyIndices &in_indices, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result)
    : flags(in_flags),
      device(in_device),
      indices(in_indices),
      MAX_FRAMES_IN_FLIGHT(in_MAX_FRAMES_IN_FLIGHT)
    {
        // Command pool creation

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

        result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
        if (result) {
            print_colored("/// WARN /// - Failed to create Command Pool", YELLOW);
            return;
        }

        // Command buffer creation

        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
        if (result) {
            print_colored("/// WARN /// - Failed to create Command Buffers (" + to_string(MAX_FRAMES_IN_FLIGHT) + ")", YELLOW);
            return;
        }

    }

    VKCCommandManager::~VKCCommandManager() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyCommandPool(device, commandPool, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Command Pool", CYAN);
    }

} // VKChis