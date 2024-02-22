//
// Created by loochis on 2/16/24.
//

#include "Utils/ColorMessages.h"
#include "Utils/VKCEnumerations.h"
#include "VKCBuffer.h"

namespace VKChis {
    VKCBuffer::VKCBuffer(uint32_t in_flags,
                         VkDevice in_device,
                         VkPhysicalDevice in_physDevice,
                         VkDeviceSize size,
                         VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags properties,
                         VkResult &result)

        : flags(in_flags),
          device(in_device),
          physDevice(in_physDevice)
        {

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to create buffer", YELLOW);
            return;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to allocate memory for buffer", YELLOW);
            return;
        }

        result = vkBindBufferMemory(device, buffer, bufferMemory, 0);
        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to bind buffer to memory", YELLOW);
            return;
        }
    }

    uint32_t VKCBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    VKCBuffer::~VKCBuffer() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyBuffer(device, buffer, nullptr);
        vkFreeMemory(device, bufferMemory, nullptr);

        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Buffer", WHITE);
    }
} // VKChis