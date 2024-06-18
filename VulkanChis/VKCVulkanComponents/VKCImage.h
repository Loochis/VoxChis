//
// Created by loochis on 6/15/24.
//

#ifndef VOXCHIS_VKCIMAGE_H
#define VOXCHIS_VKCIMAGE_H

#include "VKCDevice.h"
#include <vulkan/vulkan.h>

using namespace VKChis;
using namespace std;

class VKCImage {
public:

    VkImage image;
    VkImageView imageView;
    VkDeviceMemory imageMemory;

    VKCImage(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, uint width, uint height, uint depth, VkFormat format, VkImageAspectFlags aspectMask);
    ~VKCImage();
private:
    uint32_t flags;

    shared_ptr<VKCDevice> device;
};


#endif //VOXCHIS_VKCIMAGE_H
