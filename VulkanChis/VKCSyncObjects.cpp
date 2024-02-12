//
// Created by loochis on 2/11/24.
//

#include "VKCSyncObjects.h"
#include "Utils/ColorMessages.h"
#include "Utils/VKCEnumerations.h"

namespace VKChis {
    VKCSyncObjects::VKCSyncObjects(uint32_t in_flags, VkDevice in_device, VkResult &result)
    : flags(in_flags),
      device(in_device)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
        if (result) {
            print_colored("/// WARN /// - Failed to create image semaphore", YELLOW);
            return;
        }

        result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
        if (result) {
            print_colored("/// WARN /// - Failed to create render semaphore", YELLOW);
            return;
        }

        result = vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence);
        if (result) {
            print_colored("/// WARN /// - Failed to create in-flight fence", YELLOW);
            return;
        }
    }

    VKCSyncObjects::~VKCSyncObjects() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
        vkDestroyFence(device, inFlightFence, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed SyncObject", WHITE);
    }
} // VKChis