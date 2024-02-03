//
// Created by loochis on 2/3/24.
//

#include "VKCManager.h"

#include <memory>
#include <utility>

namespace VKChis {
    VKCManager::VKCManager(std::shared_ptr<WINChisInstance> in_window, vkc_InitFlags in_flags)
    : window(std::move(in_window)), flags(in_flags)
    {

        VkResult result;

        // Initialize instance
        instance = std::make_unique<VKCInstance>(flags, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// - Failed to create VKInstance");
        else        std::cout << "/// GOOD /// - Created VKInstance"  << std::endl;

        // Initialize Surface
        surface = std::make_unique<VKCSurface>(instance->instance, window->window, result);
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


        std::cout << "/// DONE /// - "  << std::endl;
    }

    VKCManager::~VKCManager() = default;
} // VKChis