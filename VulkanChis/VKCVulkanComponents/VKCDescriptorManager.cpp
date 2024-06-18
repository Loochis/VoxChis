//
// Created by loochis on 3/5/24.
//

#include <cstring>
#include "VKCDescriptorManager.h"
#include "../Headers/ColorMessages.h"

namespace VKChis {
    VKCDescriptorManager::VKCDescriptorManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCAssetManager> &in_assetManager, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result)
    :   flags(in_flags),
        device(in_device),
        assetManager(in_assetManager),
        MAX_FRAMES_IN_FLIGHT(in_MAX_FRAMES_IN_FLIGHT)
    {
        // Calculate required alignment based on minimum device offset alignment
        VkPhysicalDeviceProperties deviceProps;
        vkGetPhysicalDeviceProperties(device->physicalDevice, &deviceProps);
        size_t minUboAlignment = deviceProps.limits.minUniformBufferOffsetAlignment;

        // Create the layouts
        // CameraMatrix UBO Layout
        VkDescriptorSetLayoutBinding uboLayout{};
        uboLayout.binding = 0;
        uboLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayout.descriptorCount = 1;
        uboLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayout.pImmutableSamplers = nullptr; // Optional

        // Texture sampler Layout
        VkDescriptorSetLayoutBinding samplerLayout{};
        samplerLayout.binding = 1;
        samplerLayout.descriptorCount = 1;
        samplerLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayout.pImmutableSamplers = nullptr;
        samplerLayout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayout, samplerLayout};

        // Init create info
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        // Reserve space for layouts
        //descSetLayouts.resize(2);

        // Create the descriptor set layout
        result = vkCreateDescriptorSetLayout(device->device, &layoutInfo, nullptr, &descSetLayout);

        // Create the descriptor pool
        // Poolsize specifies max quantity of a *type* of descriptor
        vector<VkDescriptorPoolSize> poolSizes = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) },
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) }, // TEX SAMPLER
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) }  // IMGUI font pool
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.poolSizeCount = poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();

        poolInfo.maxSets = poolSizes.size();

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
        uniform_buffers.reserve(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.reserve(MAX_FRAMES_IN_FLIGHT);
        descriptorSets.reserve(MAX_FRAMES_IN_FLIGHT);

        VkDeviceSize camBufferSize = sizeof(CameraMatrixUBO);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descSetLayout;

        // Allocate descriptor sets

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            result = vkAllocateDescriptorSets(device->device, &allocInfo, &(descriptorSets[i]));

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER)
                    print_colored("/// WARN /// - Failed to allocate descriptor sets for double buffer " + to_string(i) + "!", YELLOW);
                return;
            }

            // Allocate Camera Buffer
            uniform_buffers.emplace_back(flags, device, camBufferSize,
                                          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                          result);

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER)
                    print_colored("/// WARN /// - Uniform Buffer allocation failed!", YELLOW);
                return;
            }

            // Persistent map memory
            result = vkMapMemory(device->device, uniform_buffers[i].bufferMemory, 0, camBufferSize, 0, &(uniformBuffersMapped[i]));

            if (result) {
                if (flags & VKC_ENABLE_VALIDATION_LAYER) print_colored("/// WARN /// - Failed to map persistent memory!", YELLOW);
                return;
            }

            // Update descriptor sets

            // CAMERA buffer
            VkDescriptorBufferInfo uboInfo{};
            uboInfo.buffer = uniform_buffers[i].buffer;

            uboInfo.offset = 0;
            uboInfo.range = sizeof(CameraMatrixUBO);

            // SAMPLER buffer
            VkDescriptorImageInfo samplerInfo{};
            samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            samplerInfo.imageView = assetManager->textureImage->imageView;
            samplerInfo.sampler = assetManager->textureSampler;

            VkWriteDescriptorSet uboDescriptorWrite{};
            uboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboDescriptorWrite.dstSet = descriptorSets[i];
            uboDescriptorWrite.dstBinding = 0;
            uboDescriptorWrite.dstArrayElement = 0;

            uboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboDescriptorWrite.descriptorCount = 1;

            uboDescriptorWrite.pBufferInfo = &uboInfo;

            VkWriteDescriptorSet samplerDescriptorWrite{};
            samplerDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            samplerDescriptorWrite.dstSet = descriptorSets[i];
            samplerDescriptorWrite.dstBinding = 1;
            samplerDescriptorWrite.dstArrayElement = 0;

            samplerDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerDescriptorWrite.descriptorCount = 1;

            samplerDescriptorWrite.pImageInfo = &samplerInfo;

            vector<VkWriteDescriptorSet> descriptorWrites;
            descriptorWrites.push_back(uboDescriptorWrite);
            descriptorWrites.push_back(samplerDescriptorWrite);

            // Write buffers
            vkUpdateDescriptorSets(device->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }

    VKCDescriptorManager::~VKCDescriptorManager() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        //for (auto & descSetLayout : descSetLayouts)
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
        memcpy(uniformBuffersMapped[currentFrame], &cameraMatrix, sizeof(cameraMatrix));
        return VKC_SUCCESS;
    }
} // VKChis