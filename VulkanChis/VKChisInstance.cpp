//
// Created by loochis on 1/28/24.
//

#include "VKChisInstance.h"
#include "Utils/VKCExtensionManager.h"
#include <stdexcept>

namespace VKChis {

    void VKChisInstance::InitInstance(VkResult &result) {
        if (enableValidationLayers && !VKChisValidator::checkValidationLayerSupport()) {
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
        if (enableValidationLayers) // Add debug utils extension if validating
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        // Add extensions to createinfo
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();


        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            debugMessenger = VKCDebugMessenger(instance, result);
            if (result) {
                std::cout << "/// WARNING /// - Failed to create debugMessenger (" << std::to_string(result) << ")"
                          << std::endl;
            }

            createInfo.pNext = &(debugMessenger.debugCreateInfo);
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        // Create Instance
        result = vkCreateInstance(&createInfo, nullptr, &instance);
    }

    // Indirect initializtion
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    VKChisInstance::VKChisInstance(WINChis::WINChisInstance &in_window, bool enableValidation) {
        window = &in_window;
        enableValidationLayers = enableValidation;

        VkResult result;

        // Initialize instance
        InitInstance(result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// - Failed to create VKInstance");
        else        std::cout << "/// GOOD /// - Created VKInstance"  << std::endl;

        // Initialize Surface
        surface = VKChisSurface(instance, window->window, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// - Failed to create Surface!");
        else        std::cout << "/// GOOD /// - Created Surface"  << std::endl;

        // Create Physical Device

        // Create Logical Device

        // Create Swap Chain

        // Create Image Views

        // Create Render Pass

        // Create GFX Pipeline

        // Create Command Pool

        // Create Command Buffers

        // Create Sync Objects

    }

    VKChisInstance::~VKChisInstance() {
        // CLEANUP POINTER REFS
        //delete(surface);

        vkDestroyInstance(instance, nullptr);
    }

#pragma clang diagnostic pop
} // VKChis