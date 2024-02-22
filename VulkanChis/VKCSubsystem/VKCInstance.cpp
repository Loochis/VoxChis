//
// Created by loochis on 1/28/24.
//

#include "VKCInstance.h"
#include <stdexcept>
#include "../Utils/ColorMessages.h"

namespace VKChis {

    VKCInstance::VKCInstance(uint32_t in_flags, const std::vector<const char*> &validationLayers, VkResult &result)
    :   flags(in_flags)
    {

        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        if (enableValidation && !VKCValidator::checkValidationLayerSupport(validationLayers)) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        // VK application info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // VK instance creation info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Get extensions required by GLFW
        auto extensions = getRequiredExtensions();
        if (enableValidation) // Add debug utils extension if validating
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        // Add extensions to createinfo
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();


        if (enableValidation) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            // SEGFAULT HERE
            debugMessenger = std::make_unique<VKCDebugMessenger>(instance, result);

            if (result)
                print_colored("/// WARNING /// - Failed to create debugMessenger", YELLOW);

            createInfo.pNext = &(debugMessenger->debugCreateInfo);
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        // Create Instance
        result = vkCreateInstance(&createInfo, nullptr, &instance);
    }

    std::vector<const char *> VKCInstance::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        return extensions;
    }


    VKCInstance::~VKCInstance() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyInstance(instance, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Instance", CYAN);
    }
} // VKChis