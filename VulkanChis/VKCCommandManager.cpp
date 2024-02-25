//
// Created by loochis on 2/11/24.
//

#include "VKCCommandManager.h"
#include "Utils/ColorMessages.h"
#include "Utils/VKCEnumerations.h"

namespace VKChis {
    VKCCommandManager::VKCCommandManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result)
    : flags(in_flags),
      device(in_device),
      indices(in_device->indices),
      MAX_FRAMES_IN_FLIGHT(in_MAX_FRAMES_IN_FLIGHT)
    {
        // Command pool creation

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

        result = vkCreateCommandPool(device->device, &poolInfo, nullptr, &commandPool);
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

        result = vkAllocateCommandBuffers(device->device, &allocInfo, commandBuffers.data());
        if (result) {
            print_colored("/// WARN /// - Failed to create Command Buffers (" + to_string(MAX_FRAMES_IN_FLIGHT) + ")", YELLOW);
            return;
        }

    }

    VKCCommandManager::~VKCCommandManager() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyCommandPool(device->device, commandPool, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Command Pool", CYAN);
    }

    // Copies one buffer to another
    void VKCCommandManager::CMD_CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        // Alloc info
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        // Create the buffer
        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

        // wait for idle instead of using a fence, if doing this multiple times consecutively use a fence instead
        vkQueueWaitIdle(device->graphicsQueue);

        vkFreeCommandBuffers(device->device, commandPool, 1, &commandBuffer);
    }

} // VKChis