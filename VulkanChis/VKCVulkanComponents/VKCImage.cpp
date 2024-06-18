//
// Created by loochis on 6/15/24.
//

#include <stdexcept>
#include "VKCImage.h"
#include "VKCBuffer.h"

VKCImage::VKCImage(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, uint width, uint height, uint depth, VkFormat format, VkImageAspectFlags aspectMask)
:   device(in_device)
{
    // image definition
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    if (depth == 1)
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
    else
        imageInfo.imageType = VK_IMAGE_TYPE_3D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = depth;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;

    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // TODO: Use this for sparse textures

    // create the image
    if (vkCreateImage(device->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VKCBuffer::findMemoryType(device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device->device, image, imageMemory, 0);

    // CREATE THE IMAGE VIEW
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    if (depth == 1)
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    else
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectMask;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

VKCImage::~VKCImage() {
    vkDestroyImageView(device->device, imageView, nullptr);
    vkDestroyImage(device->device, image, nullptr);
    vkFreeMemory(device->device, imageMemory, nullptr);
}
