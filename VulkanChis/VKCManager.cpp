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

        string shader_dir = "/home/loochis/CLionProjects/VoxChis/Shaders/";
        std::vector<std::string> shader_loadstrs {"shader.frag", "shader.vert"};

        shader_modules = make_shared<vector<VKCShaderModule>>();
        shader_modules->reserve(shader_loadstrs.size());

        for (int i = 0; i < shader_loadstrs.size(); i++) {
            string shader_path = shader_dir + shader_loadstrs[i];
            shader_modules->emplace_back(flags, shader_path, device->device, result);
            if (enableValidation) {
                if (result) print_colored("/// WARN /// - Could not load shader " + (*shader_modules)[i].comp_visname, YELLOW);
                else print_colored("/// INFO /// - Loaded " + (*shader_modules)[i].comp_visname, WHITE);
            }
        }


        graphicsPipeline = make_unique<VKCGraphicsPipeline>(flags, shader_modules, swapChain->swapChainExtent, device->device, renderPass->renderPass, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create GFX Pipeline!");
        if (enableValidation)  print_colored("/// GOOD /// - Created GFX Pipeline", GREEN);

        // Create Command Pool

        commandManager = make_unique<VKCCommandManager>(flags, device->device, device->indices, MAX_FRAMES_IN_FLIGHT, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Command Manager!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Command Manager", GREEN);

        // Create Sync Objects

        sync_objects = make_shared<vector<VKCSyncObjects>>();
        sync_objects->reserve(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            sync_objects->emplace_back(flags, device->device, result);
            if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Sync Object ( " + to_string(i) + ")!");
        }

        if (enableValidation) print_colored("/// INFO /// - Created SyncObjects (" + to_string(MAX_FRAMES_IN_FLIGHT) + ")", WHITE);

        // DONE //
        if (enableValidation) print_colored("\n/// DONE INITIALIZATION ///\n", MAGENTA);
    }

    void VKCManager::DrawFrame() {
        vkWaitForFences(device->device, 1, &(*sync_objects)[currentFrame].inFlightFence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device->device, swapChain->swapChain, UINT64_MAX, (*sync_objects)[currentFrame].imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        // Only reset the fence if we are submitting work
        vkResetFences(device->device, 1, &((*sync_objects)[currentFrame].inFlightFence));

        vkResetCommandBuffer(commandManager->commandBuffers[currentFrame],  0);
        RecordCommandBuffer(commandManager->commandBuffers[currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {(*sync_objects)[currentFrame].imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(commandManager->commandBuffers[currentFrame]);

        VkSemaphore signalSemaphores[] = {(*sync_objects)[currentFrame].renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, (*sync_objects)[currentFrame].inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain->swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(device->presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            RecreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VKCManager::RecordCommandBuffer(VkCommandBuffer commandBufferIn, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBufferIn, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->renderPass;
        renderPassInfo.framebuffer = swapChain->swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->swapChainExtent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBufferIn, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBufferIn, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->swapChainExtent.width);
        viewport.height = static_cast<float>(swapChain->swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBufferIn, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain->swapChainExtent;
        vkCmdSetScissor(commandBufferIn, 0, 1, &scissor);

        vkCmdDraw(commandBufferIn, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBufferIn);

        if (vkEndCommandBuffer(commandBufferIn) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void VKCManager::RecreateSwapChain() {
        // Wait for device idle
        vkDeviceWaitIdle(device->device);

        // Disable validation flags to stop console spam
        if (swapChain->flags & VKC_ENABLE_VALIDATION_LAYER)
            swapChain->flags -= VKC_ENABLE_VALIDATION_LAYER;
        uint32_t t_flags = swapChain->flags;

        swapChain.reset();
        VkResult result;

        swapChain = std::make_unique<VKCSwapChain>(t_flags, surface->surface, device->device, window->window, device->indices, device->swapChainSupport, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to recreate SwapChain!");

        // Recreate framebuffer
        swapChain->createFrameBuffers(renderPass->renderPass);

        // Re-instate the previously disabled validation flag
        if (flags & VKC_ENABLE_VALIDATION_LAYER)
            swapChain->flags += VKC_ENABLE_VALIDATION_LAYER;
    }

    VKCManager::~VKCManager() {
        // Wait for device to be idle before destroying
        vkDeviceWaitIdle(device->device);
    }
} // VKChis