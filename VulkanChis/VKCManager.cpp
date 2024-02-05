//
// Created by loochis on 2/3/24.
//

#include "VKCManager.h"
#include "Utils/ColorMessages.h"

#include <memory>
#include <utility>

using namespace std;

namespace VKChis {
    VKCManager::VKCManager(std::shared_ptr<WINChisInstance> in_window, uint32_t in_flags)
    : window(std::move(in_window)), flags(in_flags)
    {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        VkResult result;

        // Initialize instance
        instance = std::make_unique<VKCInstance>(flags, validationLayers, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// - Failed to create VKInstance");
        if (enableValidation)  print_colored("/// GOOD /// - Created VKInstance", GREEN);

        // Initialize Surface
        surface = std::make_unique<VKCSurface>(flags, instance->instance, window->window, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// - Failed to create Surface!");
        if (enableValidation)  print_colored("/// GOOD /// - Created VKSurfaceKHR", GREEN);

        // Create Logical Device
        device = std::make_unique<VKCLogicalDevice>(flags, validationLayers, deviceExtensions, instance->instance, surface->surface, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Logical Device!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Logical Device", GREEN);

        // Create Swap Chain
        swapChain = std::make_unique<VKCSwapChain>(flags, surface->surface, device->device, window->window, device->indices, device->swapChainSupport, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create SwapChain!");
        if (enableValidation)  print_colored("/// GOOD /// - Created SwapChain", GREEN);

        // Create Render Pass
        renderPass = std::make_unique<VKCRenderPass>(flags, device->device, swapChain->swapChainImageFormat, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Render Pass!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Render Pass", GREEN);

        // Create Framebuffer
        swapChain->createFrameBuffers(renderPass->renderPass);

        // Create GFX Pipeline

        // Create Command Pool

        // Create Command Buffers

        // Create Sync Objects


        if (enableValidation) print_colored("\n/// DONE INITIALIZATION ///\n", MAGENTA);
    }

    VKCManager::~VKCManager() = default;
} // VKChis