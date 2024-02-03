//
// Created by loochis on 1/29/24.
//

#ifndef VOXCHIS_VKCDEBUGMESSENGER_H
#define VOXCHIS_VKCDEBUGMESSENGER_H

#include <vulkan/vulkan.h>
#include <iostream>

namespace VKChis {
    class VKCDebugMessenger {
    public:
        // STATIC HELPERS
        static VKAPI_ATTR VkBool32 VKAPI_CALL
        debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        // CONSTRUCTOR
        VKCDebugMessenger(VkInstance in_instance, VkResult &result);

        // DESTRUCTOR
        ~VKCDebugMessenger();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
    };
}

#endif //VOXCHIS_VKCDEBUGMESSENGER_H
