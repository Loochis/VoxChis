//
// Created by loochis on 6/15/24.
//

#ifndef VOXCHIS_VKCIMAGE_H
#define VOXCHIS_VKCIMAGE_H

#include "VKCDevice.h"
#include "VKCCommandManager.h"
#include <vulkan/vulkan.h>

using namespace VKChis;
using namespace std;

class VKCImage {
public:

    VkImage image;
    VkImageView imageView;
    VkDeviceMemory imageMemory;

    VkFormat format;

    VKCImage(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, uint width, uint height, uint depth, VkFormat in_format, VkImageUsageFlags usage, VkImageAspectFlags aspectMask);
    ~VKCImage();

    VkResult transitionImageLayout(shared_ptr<VKCCommandManager> &commandManager, VkImageLayout oldLayout, VkImageLayout newLayout);
    static bool hasStencilComponent(VkFormat format);
private:
    uint32_t flags;

    shared_ptr<VKCDevice> device;
};


#endif //VOXCHIS_VKCIMAGE_H
