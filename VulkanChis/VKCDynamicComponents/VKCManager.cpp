//
// Created by loochis on 2/3/24.
//

#include "VKCManager.h"
#include "../Utils/ColorMessages.h"

#include <memory>
#include <utility>

using namespace std;

namespace VKChis {

    const std::vector<Vertex> vertices = {
            {{ 1.0f, -1.0f, -1.0f}},
            {{ 1.0f, -1.0f,  1.0f}},
            {{-1.0f, -1.0f,  1.0f}},
            {{-1.0f, -1.0f, -1.0f}},
            {{ 1.0f,  1.0f, -1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{-1.0f,  1.0f,  1.0f}},
            {{-1.0f,  1.0f, -1.0f}}
    };

    const std::vector<uint16_t> indices = {
            1, 2, 3,
            7, 6, 5,
            4, 5, 1,
            5, 6, 2,
            2, 6, 7,
            0, 3, 7,
            0, 1, 3,
            4, 7, 5,
            0, 4, 1,
            1, 5, 2,
            3, 2, 7,
            4, 0, 7
    };

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
        device = std::make_shared<VKCDevice>(flags, validationLayers, deviceExtensions, instance->instance, surface->surface, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Logical Device!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Logical Device", GREEN);

        // Create Swap Chain
        swapChain = std::make_unique<VKCSwapChain>(flags, surface->surface, device, window->window, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create SwapChain!");
        if (enableValidation)  print_colored("/// GOOD /// - Created SwapChain", GREEN);

        // Create Render Pass
        renderPass = std::make_unique<VKCRenderPass>(flags, device, swapChain->swapChainImageFormat, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Render Pass!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Render Pass", GREEN);

        // Create Framebuffer
        swapChain->createFrameBuffers(renderPass->renderPass);

        // Create Descriptors
        descriptorManager = make_shared<VKCDescriptorManager>(flags, device, MAX_FRAMES_IN_FLIGHT, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Descriptor Pool/Set!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Descriptor Pool and Set", GREEN);

        // Create GFX Pipeline

        string shader_dir = "/home/loochis/CLionProjects/VoxChis/Shaders/";
        std::vector<std::string> shader_loadstrs {"shader.frag", "shader.vert"};

        shader_modules = make_shared<vector<VKCShaderModule>>();
        shader_modules->reserve(shader_loadstrs.size());

        for (int i = 0; i < shader_loadstrs.size(); i++) {
            string shader_path = shader_dir + shader_loadstrs[i];
            shader_modules->emplace_back(flags, shader_path, device, result);
            if (enableValidation) {
                if (result) print_colored("/// WARN /// - Could not load shader " + (*shader_modules)[i].comp_visname, YELLOW);
                else print_colored("/// INFO /// - Loaded " + (*shader_modules)[i].comp_visname, WHITE);
            }
        }

        graphicsPipeline = make_unique<VKCGraphicsPipeline>(flags, shader_modules, swapChain->swapChainExtent, device, descriptorManager, renderPass->renderPass, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create GFX Pipeline!");
        if (enableValidation)  print_colored("/// GOOD /// - Created GFX Pipeline", GREEN);

        // Create Command Pool

        commandManager = make_unique<VKCCommandManager>(flags, device, MAX_FRAMES_IN_FLIGHT, result);
        if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Command Manager!");
        if (enableValidation)  print_colored("/// GOOD /// - Created Command Manager", GREEN);

        // VERTEX BUFFER

        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        unique_ptr<VKCBuffer> stage_buffer = make_unique<VKCBuffer>(flags, device, bufferSize,
                                           VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, result);

        void* data;
        vkMapMemory(device->device, stage_buffer->bufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device->device, stage_buffer->bufferMemory);

        vert_buffer = make_unique<VKCBuffer>(flags, device, bufferSize,
                                             VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, result);

        // copy buf
        commandManager->CMD_CopyBuffer(stage_buffer->buffer, vert_buffer->buffer, bufferSize);

        // END VERTEX BUFFER

        // INDEX BUFFER

        stage_buffer.reset();
        bufferSize = sizeof(indices[0]) * indices.size();

        stage_buffer = make_unique<VKCBuffer>(flags, device, bufferSize,
                                                                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, result);

        vkMapMemory(device->device, stage_buffer->bufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(device->device, stage_buffer->bufferMemory);

        ind_buffer = make_unique<VKCBuffer>(flags, device, bufferSize,
                                             VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, result);

        // copy buf
        commandManager->CMD_CopyBuffer(stage_buffer->buffer, ind_buffer->buffer, bufferSize);

        // END INDEX BUFFER

        // Create Sync Objects

        sync_objects = make_shared<vector<VKCSyncObjects>>();
        sync_objects->reserve(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            sync_objects->emplace_back(flags, device, result);
            if (result) throw std::runtime_error("/// FATAL ERROR /// Failed to create Sync Object ( " + to_string(i) + ")!");
        }

        if (enableValidation) print_colored("/// INFO /// - Created SyncObjects (" + to_string(MAX_FRAMES_IN_FLIGHT) + ")", WHITE);

        // DONE //
        if (enableValidation) print_colored("\n/// DONE INITIALIZATION ///\n", MAGENTA);
    }

    // TEMP

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

        // Update the UBO
        updateUniformBuffer(currentFrame);

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

    void VKCManager::updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        viewMat = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        projMat = glm::perspective(glm::radians(45.0f), swapChain->swapChainExtent.width / (float) swapChain->swapChainExtent.height, 0.1f, 10.0f);
        projMat[1][1] *= -1;

        obj1Mat = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0));
        obj1Mat = glm::rotate(obj1Mat, time * glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
        obj1Mat = glm::scale(obj1Mat, glm::vec3(1.0f, 1.0f, 1.0f));


        //obj1Mat = projMat * viewMat * obj1Mat;
        obj2Mat = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0));
        obj2Mat = glm::rotate(obj2Mat, time * glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        descriptorManager->cameraMatrix.vp_mat = projMat * viewMat;
        descriptorManager->UpdateUBOs(currentImage);
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

        vkCmdBindPipeline(commandBufferIn, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

        VkBuffer vertexBuffers[] = {vert_buffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBufferIn, 0, 1, vertexBuffers, offsets);

        // INDICES
        vkCmdBindIndexBuffer(commandBufferIn, ind_buffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        PushConstData pushConstData{};
        glm::vec3 camPos;

        // bind global consts
        vkCmdBindDescriptorSets(commandBufferIn, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, descriptorManager->descriptorSets[currentFrame].data(), 0,
                                nullptr);

        camPos = glm::vec3(glm::inverse(obj1Mat) * glm::vec4(0, 5, 5, 1));
        pushConstData.m_mat = obj1Mat;
        pushConstData.im_campos = camPos;

        // draw object 1
        vkCmdPushConstants(commandBufferIn, graphicsPipeline->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstData), &pushConstData);
        vkCmdDrawIndexed(commandBufferIn, static_cast<uint32_t>(36), 1, 0, 0, 0);

        camPos = glm::vec3(glm::inverse(obj2Mat) * glm::vec4(0, 5, 5, 1));
        pushConstData.m_mat = obj2Mat;
        pushConstData.im_campos = camPos;

        // draw object 2
        vkCmdPushConstants(commandBufferIn, graphicsPipeline->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstData), &pushConstData);
        vkCmdDrawIndexed(commandBufferIn, static_cast<uint32_t>(36), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBufferIn);

        if (vkEndCommandBuffer(commandBufferIn) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void VKCManager::RecreateSwapChain() {
        // Wait for device idle
        vkDeviceWaitIdle(device->device);

        cout << "RECREATED SWAPCHAIN";

        // Disable validation flags to stop console spam
        if (swapChain->flags & VKC_ENABLE_VALIDATION_LAYER)
            swapChain->flags -= VKC_ENABLE_VALIDATION_LAYER;
        uint32_t t_flags = swapChain->flags;

        swapChain.reset();
        VkResult result;

        swapChain = std::make_unique<VKCSwapChain>(t_flags, surface->surface, device, window->window, result);
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