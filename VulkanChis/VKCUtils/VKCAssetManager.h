//
// Created by loochis on 6/8/24.
//

#ifndef VOXCHIS_VKCASSETMANAGER_H
#define VOXCHIS_VKCASSETMANAGER_H


#include <vulkan/vulkan.h>
#include "../VKCVulkanComponents/VKCBuffer.h"
#include "../VKCVulkanComponents/VKCCommandManager.h"
#include "../VKCUtils/VKCAssetLoader.h"
#include "../VKCVulkanComponents/VKCImage.h"

using namespace VKChis;

class VKCAssetManager {
public:
    // test vars
    shared_ptr<VKCImage> textureImage;
    VkSampler textureSampler;

    VKCAssetManager(uint32_t in_flags,
                    shared_ptr<VKCDevice> &in_device,
                    shared_ptr<VKCCommandManager> &in_commandManager);

    ~VKCAssetManager();

    VkResult AddModel();
    VkResult AddPalette();
private:
    uint32_t flags;
    shared_ptr<VKCDevice> device;
    shared_ptr<VKCCommandManager> commandManager;

    unique_ptr<VKCBuffer> staging_buffer;
    unique_ptr<VKCAssetLoader> loader;

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t depth);
};


#endif //VOXCHIS_VKCASSETMANAGER_H
