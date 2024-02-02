//
// Created by loochis on 1/29/24.
//
#include "VKCDebugMessenger.h"

using namespace VKChis;

VkBool32 VKCDebugMessenger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                          void *pUserData) {

        std::cout << "VKC Validation: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
}

void VKCDebugMessenger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)  {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VKCDebugMessenger::VKCDebugMessenger() = default;

// clang whines about not initializing debugCreateInfo and debugMessenger. They get set but indirectly
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
VKCDebugMessenger::VKCDebugMessenger(VkInstance &in_instance, VkResult &result) {
    instance = in_instance;

    populateDebugMessengerCreateInfo(debugCreateInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        result = func(instance, &debugCreateInfo, nullptr, &debugMessenger);
    } else {
        result = VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
#pragma clang diagnostic pop

VKCDebugMessenger::~VKCDebugMessenger() {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, nullptr);
    }
}

