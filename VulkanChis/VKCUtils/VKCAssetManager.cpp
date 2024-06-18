//
// Created by loochis on 6/8/24.
//

#include <cstring>
#include "VKCAssetManager.h"
#include "../Headers/ColorMessages.h"

VKCAssetManager::VKCAssetManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCCommandManager> &in_commandManager)
        : flags(in_flags),
          device(in_device),
          commandManager(in_commandManager)
          {

}

VKCAssetManager::~VKCAssetManager() {
    vkDestroySampler(device->device, textureSampler, nullptr);
}

VkResult VKCAssetManager::AddModel() {

    VkResult result;

    // Load the file
    loader = make_unique<VKCAssetLoader>(flags);
    string vox_dir = "/home/loochis/CLionProjects/VoxChis/Assets/Vox/chr_knight.vox";
    loader->LoadVOX(vox_dir);

    VkDeviceSize imageSize = loader->dimensions[0]*loader->dimensions[1]*loader->dimensions[2];

    staging_buffer = make_unique<VKCBuffer>(imageSize, device, imageSize,
                                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, result);
    if (result) {
        if (flags & VKC_ASSET_LOAD_DEBUG) print_colored("/// WARN /// - Failed to create model staging buffer", YELLOW);
        return result;
    }

    // Copy the pixels into the staging buffer
    void* data;
    vkMapMemory(device->device, staging_buffer->bufferMemory, 0, imageSize, 0, &data);
    memcpy(data, loader->voxel_data.data(), static_cast<size_t>(imageSize));
    vkUnmapMemory(device->device, staging_buffer->bufferMemory);

    textureImage = make_shared<VKCImage>(flags, device,
                                         loader->dimensions[0], loader->dimensions[1], loader->dimensions[2],
                                         VK_FORMAT_R8_UINT, VK_IMAGE_ASPECT_COLOR_BIT);


    transitionImageLayout(textureImage->image, VK_FORMAT_R8_UINT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(staging_buffer->buffer, textureImage->image, loader->dimensions[0], loader->dimensions[1], loader->dimensions[2]);

    // another transition for shader read optimisation
    transitionImageLayout(textureImage->image, VK_FORMAT_R8_UINT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // destroy the staging buffer
    staging_buffer.reset();

    // CREATE THE SAMPLER
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device->device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }

    print_colored("/// ASSET DEBUG /// - Added Model", GREEN);

    return result;
}

VkResult VKCAssetManager::AddPalette() {
    VkResult result;
    return result;
}

// TODO: Move this functionality somewhere proper
void VKCAssetManager::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                            VkImageLayout newLayout) {

    commandManager->SingleCommandBuffer_Start();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
            commandManager->commandBuffer_single,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
    );

    commandManager->SingleCommandBuffer_End();
}

void VKCAssetManager::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t depth) {
    commandManager->SingleCommandBuffer_Start();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
            width,
            height,
            depth
    };

    vkCmdCopyBufferToImage(
            commandManager->commandBuffer_single,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
    );

    commandManager->SingleCommandBuffer_End();
}
