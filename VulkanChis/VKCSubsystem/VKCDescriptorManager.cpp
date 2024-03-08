//
// Created by loochis on 3/5/24.
//

#include <cstring>
#include "VKCDescriptorManager.h"
#include "../Utils/ColorMessages.h"

namespace VKChis {
    VKCDescriptorManager::VKCDescriptorManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCDescriptorSetLayout> &in_descLayout, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result)
    :   device(in_device),
        descLayout(in_descLayout),
        MAX_FRAMES_IN_FLIGHT(in_MAX_FRAMES_IN_FLIGHT)
    {
        // Create the layouts
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;

        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        result = vkCreateDescriptorSetLayout(device->device, &layoutInfo, nullptr, &cameraSetLayout);

        // Create the descriptor pool
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        result = vkCreateDescriptorPool(device->device, &poolInfo, nullptr, &descriptorPool);

        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Descriptor Pool allocation failed!", YELLOW);
            return;
        }

        // Init buffers vec
        uniform_buffers = make_unique<vector<VKCBuffer>>();
    }

    VKCDescriptorManager::~VKCDescriptorManager() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Descriptor Pool", CYAN);
    }

    // Creates and allocates an arbitrary UBO to facilitate object abstraction
    // TODO: Generify binding index and buffer struct
    vkc_Result VKCDescriptorManager::AllocateObjectDescriptors() {

        // Create the descriptor sets
        // Set layout information will be preserved until this object is destroyed AND VKCManager uninitializes
        vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descLayout->descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        // 2D vector of (per object/per framebuffer) descriptors
        // This might be a perf hit if there are more frames in flight
        descriptorSets.emplace_back();
        ulong elem_idx = descriptorSets.size()-1;

        descriptorSets[elem_idx].resize(MAX_FRAMES_IN_FLIGHT);

        VkResult result = vkAllocateDescriptorSets(device->device, &allocInfo, descriptorSets[elem_idx].data());

        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Descriptor Set allocation failed!", YELLOW);
            return VKC_ALLOCATION_FAILURE;
        }

        // Create the uniform buffer (single buffer contains all flightframe-based data)
        ulong bufferSize = sizeof(UniformBufferObject) * MAX_FRAMES_IN_FLIGHT;

        uniform_buffers->emplace_back(flags, device, bufferSize,
                                      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, result);

        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Uniform Buffer allocation failed!", YELLOW);
            return VKC_ALLOCATION_FAILURE;
        }

        // Persistent map the buffer
        uniformBuffersMapped.emplace_back();
        result = vkMapMemory(device->device, (*uniform_buffers)[elem_idx].bufferMemory, 0, bufferSize, 0, &uniformBuffersMapped[elem_idx]);

        if (result) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to map persistent memory!", YELLOW);
            return VKC_ALLOCATION_FAILURE;
        }

        // Update the objects descriptor sets
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = (*uniform_buffers)[elem_idx].buffer;

            // Store frame-based buffers in single buffer with offsets
            bufferInfo.offset = i*sizeof(UniformBufferObject);
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[elem_idx][i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;

            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;

            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(device->device, 1, &descriptorWrite, 0, nullptr);
        }

        return VKC_SUCCESS;
    }

    vkc_Result VKCDescriptorManager::UpdateCameraUBOData(CameraMatrixUBO &ubo, int obj, int currentFrame) {
        // pointer offset to frame-based buffer
        memcpy(uniformBuffersMapped[obj], &ubo, sizeof(ubo));
        return VKC_SUCCESS;
    }
} // VKChis