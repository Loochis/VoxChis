//
// Created by loochis on 3/5/24.
//

#include <cstring>
#include "VKCDescriptorManager.h"
#include "../Headers/ColorMessages.h"

namespace VKChis {
    VKCDescriptorManager::VKCDescriptorManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result)
    :   flags(in_flags),
        device(in_device),
        MAX_FRAMES_IN_FLIGHT(in_MAX_FRAMES_IN_FLIGHT)
    {
        // Calculate required alignment based on minimum device offset alignment
        VkPhysicalDeviceProperties deviceProps;
        vkGetPhysicalDeviceProperties(device->physicalDevice, &deviceProps);
        size_t minUboAlignment = deviceProps.limits.minUniformBufferOffsetAlignment;

        // Create the layouts
        // CameraMatrix UBO Layout
        VkDescriptorSetLayoutBinding camMatrixLayout{};
        camMatrixLayout.binding = 0;
        camMatrixLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        camMatrixLayout.descriptorCount = 1;
        camMatrixLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        camMatrixLayout.pImmutableSamplers = nullptr; // Optional

        // ModelMatrix UBO Layout
        VkDescriptorSetLayoutBinding modelMatrixLayout{};
        modelMatrixLayout.binding = 0;
        modelMatrixLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        modelMatrixLayout.descriptorCount = 1;
        modelMatrixLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        modelMatrixLayout.pImmutableSamplers = nullptr; // Optional

        // Init create info
        VkDescriptorSetLayoutCreateInfo camLayoutInfo{};
        camLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        camLayoutInfo.bindingCount = 1;
        camLayoutInfo.pBindings = &camMatrixLayout;

        VkDescriptorSetLayoutCreateInfo modLayoutInfo{};
        modLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        modLayoutInfo.bindingCount = 1;
        modLayoutInfo.pBindings = &modelMatrixLayout;

        // Reserve space for layouts
        descSetLayouts.resize(2);

        // Create the descriptor set layout
        result = vkCreateDescriptorSetLayout(device->device, &camLayoutInfo, nullptr, &(descSetLayouts[0]));
        result = vkCreateDescriptorSetLayout(device->device, &modLayoutInfo, nullptr, &(descSetLayouts[1]));

        // Create the descriptor pool
        // Poolsize specifies max quantity of a *type* of descriptor
        vector<VkDescriptorPoolSize> poolSizes = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * 2},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * 2}  // IMGUI font pool
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();

        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * 4;

        result = vkCreateDescriptorPool(device->device, &poolInfo, nullptr, &descriptorPool);

        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Descriptor Pool allocation failed!", YELLOW);
            return;
        }

        // Uniform buffer layout:
        // Double Buffer > DB_0    | DB_1
        // UBO           > UBO_CAM | UBO_CAM
        // Mapped Index  > 0,0     | 1,0

        // Init uniform buffers/desc set vector resources
        uniform_buffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            uniform_buffers[i].reserve(1);
            uniformBuffersMapped[i].resize(1);
            descriptorSets[i].resize(1);
        }

        VkDeviceSize camBufferSize = sizeof(CameraMatrixUBO);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = descSetLayouts.size();
        allocInfo.pSetLayouts = descSetLayouts.data();

        // Allocate descriptor sets

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            result = vkAllocateDescriptorSets(device->device, &allocInfo, descriptorSets[i].data());

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER)
                    print_colored("/// WARN /// - Failed to allocate descriptor sets for double buffer " + to_string(i) + "!", YELLOW);
                return;
            }

            // Allocate Camera Buffer
            uniform_buffers[i].emplace_back(flags, device, camBufferSize,
                                          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                          result);

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER)
                    print_colored("/// WARN /// - Uniform Buffer allocation failed!", YELLOW);
                return;
            }

            // Persistent map memory
            result = vkMapMemory(device->device, uniform_buffers[i][0].bufferMemory, 0, camBufferSize, 0, &(uniformBuffersMapped[i][0]));

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to map persistent memory!", YELLOW);
                return;
            }

            // Update descriptor sets

            // CAMERA buffer
            VkDescriptorBufferInfo camBufferInfo{};
            camBufferInfo.buffer = uniform_buffers[i][0].buffer;

            camBufferInfo.offset = 0;
            camBufferInfo.range = sizeof(CameraMatrixUBO);

            VkWriteDescriptorSet camDescriptorWrite{};
            camDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            camDescriptorWrite.dstSet = descriptorSets[i][0];
            camDescriptorWrite.dstBinding = 0;
            camDescriptorWrite.dstArrayElement = 0;

            camDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            camDescriptorWrite.descriptorCount = 1;

            camDescriptorWrite.pBufferInfo = &camBufferInfo;
            camDescriptorWrite.pImageInfo = nullptr; // Optional
            camDescriptorWrite.pTexelBufferView = nullptr; // Optional

            vector<VkWriteDescriptorSet> descriptorWrites;
            descriptorWrites.push_back(camDescriptorWrite);

            // Write buffers
            vkUpdateDescriptorSets(device->device, 1, descriptorWrites.data(), 0, nullptr);
        }
    }

    VKCDescriptorManager::~VKCDescriptorManager() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        for (auto & descSetLayout : descSetLayouts)
            vkDestroyDescriptorSetLayout(device->device, descSetLayout, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Descriptor Set Layouts", CYAN);

        vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Descriptor Pool", CYAN);
    }

    // Creates and allocates an arbitrary UBO to facilitate object abstraction
    // TODO: Generify binding index and buffer struct
    vkc_Result VKCDescriptorManager::AllocateObjectDescriptors() {

        return VKC_SUCCESS;
    }

    vkc_Result VKCDescriptorManager::UpdateUBOs(int currentFrame) {
        memcpy(uniformBuffersMapped[currentFrame][0], &cameraMatrix, sizeof(cameraMatrix));
        return VKC_SUCCESS;
    }
} // VKChis